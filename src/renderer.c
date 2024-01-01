#include "renderer.h"

#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/syscall.h>

#include "shader.h"
#include "linux/input.h"
#include "time_utils.h"
#include "rasterizer.h"

/********************
 *  Notes
 *
 ********************/

/********************/
/*      defines     */
/********************/

/********************/
/* static variables */
/********************/

typedef struct
{
    uint32_t    offset;
    uint32_t    size;

    uint32_t    width;
    uint32_t    height;

    mesh_t*     mesh;
    camera_t*   camera;

    uint32_t    color;
} triangle_batch_t;

static uint32_t width              = 800;
static uint32_t height             = 600;

static scene_t* scene              = NULL;
static display_t* display          = NULL;

static framebuffer_t* current      = NULL;
static framebuffer_t* front        = NULL;
static framebuffer_t* back         = NULL;

static depthbuffer_t* depthbuffer  = NULL;

static bool wireframe              = false;

static thread_pool_t* thread_pool  = NULL;
static triangle_batch_t thread_data[THREAD_COUNT] = { 0 };

/********************/
/* static functions */
/********************/

static void renderer_draw_utilities()
{
    camera_t* cam   = scene->camera;

    vec_t points[4] = {vec_new(0.f, 0.f, 0.f),
                       vec_new(1.f, 0.f, 0.f),
                       vec_new(0.f, 1.f, 0.f),
                       vec_new(0.f, 0.f, 1.f)};

    uint32_t colors[4] = {0x00000000, 
                          0x0000FF00, 
                          0x00FF0000, 
                          0xFF000000};

    mat_t PV = mat_mul_mat(camera_proj_transform(cam),
                           camera_view_transform(cam));

    for (uint32_t i = 0; i < sizeof(points) / sizeof(vec_t); i++)
    {
        points[i]   = mat_mul_vec(PV, points[i]);
        points[i]   = vec_scale(points[i], 1.f/points[i].w);
        points[i].x = (points[i].x + 1.f) * 0.5f * (float)width;
        points[i].y = (points[i].y + 1.f) * 0.5f * (float)height;
    }

    rasterizer_draw_line(points[0], points[1], colors[1], current);
    rasterizer_draw_line(points[0], points[2], colors[2], current);
    rasterizer_draw_line(points[0], points[3], colors[3], current);
}

static void renderer_process_triangles(void* args)
{
    triangle_batch_t* batch = (triangle_batch_t*)args;

    uint32_t offset = batch->offset;
    uint32_t size   = batch->size;

    float w_over_2  = (float)batch->width * 0.5f;
    float h_over_2  = (float)batch->height * 0.5f;

    mesh_t* m       = batch->mesh;
    camera_t* c     = batch->camera;

    for (uint32_t i = offset; i < offset + size; i++)
    {
        uint32_t v    = i * 3;
        triangle_t* t = &m->triangles[i];

        uint32_t i0 = m->indices[v + 0];
        uint32_t i1 = m->indices[v + 1];
        uint32_t i2 = m->indices[v + 2];

        t->v0 = m->vertices[i0];
        t->v1 = m->vertices[i1];
        t->v2 = m->vertices[i2];

        //TODO: add backface culling

        process_vertex(t, c, NULL, 0);

        // persp divide
        t->v0 = vec_scale(t->v0, 1.f / t->v0.w);
        t->v1 = vec_scale(t->v1, 1.f / t->v1.w);
        t->v2 = vec_scale(t->v2, 1.f / t->v2.w);

        // viewport transform
        t->v0.x = (t->v0.x + 1.f) * w_over_2;
        t->v0.y = (t->v0.y + 1.f) * h_over_2;
        t->v1.x = (t->v1.x + 1.f) * w_over_2;
        t->v1.y = (t->v1.y + 1.f) * h_over_2;
        t->v2.x = (t->v2.x + 1.f) * w_over_2;
        t->v2.y = (t->v2.y + 1.f) * h_over_2;

    }
}

static void renderer_draw_mesh(mesh_t* mesh)
{
    triangle_batch_t t;

    camera_t* camera    = scene->camera;
    uint32_t batch_size = mesh->indices_size / 3 / THREAD_COUNT;

    for (uint32_t i = 0; i < THREAD_COUNT; i++)
    {
        t.offset    = i * batch_size;
        t.size      = batch_size;

        t.width     = width;
        t.height    = height;

        t.mesh      = mesh;
        t.camera    = camera;

        if (i == THREAD_COUNT - 1)
        {
            t.size = (mesh->indices_size / 3) - t.offset;
        }

        thread_data[i] = t;
        thread_pool_add(thread_pool, renderer_process_triangles, &thread_data[i]);
    }

    thread_pool_wait(thread_pool);

    triangle_t* triangles   = mesh->triangles;
    uint32_t triangles_size = mesh->triangles_size;

    for (uint32_t i = 0; i < triangles_size; i++)
    {
        rasterizer_draw_triangle(triangles[i].v0,
                                 triangles[i].v1,
                                 triangles[i].v2,
                                 0xFFFFFFFF, //triangles[i].color,
                                 current,
                                 depthbuffer);
    }
}

static void renderer_update(input_t input)
{
    scene_update(scene, input);
    if (input.keys & KEY_1)
    {
        wireframe = !wireframe;
    }
}

static void renderer_draw()
{
    renderer_draw_utilities();

    renderer_draw_mesh(scene->mesh);

    display_draw(display, current);
}

static void renderer_clear_buffers()
{
    framebuffer_clear(current);
    depthbuffer_clear(depthbuffer);
    display_clear(display);
}

/********************/
/* public functions */
/********************/

void renderer_init(uint32_t w, uint32_t h)
{
    width       = w;
    height      = h;
    scene       = NULL;
    display     = display_new(width, height);
    front       = framebuffer_new(width, height);
    back        = framebuffer_new(width, height);
    current     = front;
    depthbuffer = depthbuffer_new(width, height);
    wireframe   = false;
    thread_pool = thread_pool_new("Triangle Processor");
    
    memset(thread_data, 0, sizeof(triangle_batch_t) * THREAD_COUNT);
}

void renderer_load(const char* file_path)
{
    if (scene)
    {
        scene_free(scene);
    }

    scene = scene_new(file_path);
}

void renderer_run()
{
    bool quit = false;
    timestamp_t start;
    timestamp_t end;
    timestamp_t diff;
    timestamp_t frame_time = 16 * MILLISECOND;
    struct timespec sleep_timer = { 0, 0 };

    while (!quit)
    {
        start = time_now();

        input_t input = handle_input(display);

        renderer_update(input);

        renderer_draw();

        renderer_clear_buffers();

        current = current == front ? back : front;

        // maintain 60fps
        end = time_now();
        diff = end - start;

        if (frame_time > diff)
        {
            sleep_timer.tv_nsec = frame_time - diff;
            nanosleep(&sleep_timer, NULL);
        }

        printf("%ldus\n", diff / 1000);

        // quit
        quit = input.keys & QUIT;
    }
}

void renderer_free()
{
    thread_pool_free(thread_pool);
    scene_free(scene);
    display_free(display);
    framebuffer_free(front);
    framebuffer_free(back);
    depthbuffer_free(depthbuffer);
}