#include "renderer.h"

#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/syscall.h>

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

/********************/
/* static functions */
/********************/

static void render_utils(renderer_t* renderer)
{
    float width     = (float)renderer->current->width;
    float height    = (float)renderer->current->height;
    camera_t* cam   = renderer->scene->camera;

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
        points[i].x = (points[i].x + 1.f) * 0.5f * width;
        points[i].y = (points[i].y + 1.f) * 0.5f * height;
    }

    rasterize_line(points[0], points[1], colors[1], renderer->current);
    rasterize_line(points[0], points[2], colors[2], renderer->current);
    rasterize_line(points[0], points[3], colors[3], renderer->current);
}

static void render_wireframe(renderer_t* renderer)
{
    float width     = (float)renderer->current->width;
    float height    = (float)renderer->current->height;

    mesh_t* mesh    = renderer->scene->mesh;
    camera_t* cam   = renderer->scene->camera;

    vec_t points[3];

    if (!camera_is_mesh_visible(cam, mesh->bounding_sphere))
    {
        printf("mesh not visible\n");
        return;
    }

    mat_t PV = mat_mul_mat(camera_proj_transform(cam),
                           camera_view_transform(cam));

    for (uint32_t i = 0; i < mesh->indices_size; i += 3)
    {
        uint32_t i0 = mesh->indices[i + 0];
        uint32_t i1 = mesh->indices[i + 1];
        uint32_t i2 = mesh->indices[i + 2];

        points[0] = mesh->vertices[i0];
        points[1] = mesh->vertices[i1];
        points[2] = mesh->vertices[i2];

        for (uint32_t j = 0; j < sizeof(points) / sizeof(vec_t); j++)
        {
            // mvp transform
            points[j]   = mat_mul_vec(PV, points[j]);

            // persp divide
            points[j]   = vec_scale(points[j], 1.f/points[j].w);

            // viewport transform
            points[j].x = (points[j].x + 1.f) * 0.5f * width;
            points[j].y = (points[j].y + 1.f) * 0.5f * height;
        }

        rasterize_line(points[0], points[1], 0xFFFFFFFF, renderer->current);
        rasterize_line(points[1], points[2], 0xFFFFFFFF, renderer->current);
        rasterize_line(points[2], points[0], 0xFFFFFFFF, renderer->current);
        // rasterize_triangle(points[0], points[1], points[2], 0xFFFFFFFF, renderer->current, renderer->depthbuffer);
    }
}

static void renderer_clear_buffers(renderer_t* renderer)
{
    framebuffer_clear(renderer->current);
    depthbuffer_clear(renderer->depthbuffer);
    display_clear(renderer->display);
}

static void renderer_process_triangles(void* args)
{
    triangle_batch_t* batch = (triangle_batch_t*)args;

    uint32_t offset     = batch->offset;
    uint32_t size       = batch->size;

    float width         = (float)batch->width;
    float height        = (float)batch->height;

    mesh_t* mesh        = batch->mesh;
    camera_t* camera    = batch->camera;

    vec_t points[3];

    mat_t PV = mat_mul_mat(camera_proj_transform(camera),
                           camera_view_transform(camera));

    for (uint32_t i = offset; i < offset + size; i++)
    {
        uint32_t v    = i * 3;
        triangle_t* t = &mesh->triangles[i];

        uint32_t i0 = mesh->indices[v + 0];
        uint32_t i1 = mesh->indices[v + 1];
        uint32_t i2 = mesh->indices[v + 2];

        points[0] = mesh->vertices[i0];
        points[1] = mesh->vertices[i1];
        points[2] = mesh->vertices[i2];

        for (uint32_t j = 0; j < sizeof(points) / sizeof(vec_t); j++)
        {
            // mvp transform
            points[j]   = mat_mul_vec(PV, points[j]);

            // persp divide
            points[j]   = vec_scale(points[j], 1.f/points[j].w);

            // viewport transform
            points[j].x = (points[j].x + 1.f) * 0.5f * width;
            points[j].y = (points[j].y + 1.f) * 0.5f * height;
        }

        t->v0 = points[0];
        t->v1 = points[1];
        t->v2 = points[2];
    }
}

static void renderer_draw_mesh(renderer_t* renderer, mesh_t* mesh)
{
    triangle_batch_t t;

    camera_t* camera    = renderer->scene->camera;
    uint32_t batch_size = mesh->indices_size / 3 / THREAD_COUNT;

    for (uint32_t i = 0; i < THREAD_COUNT; i++)
    {
        t.offset    = i * batch_size;
        t.size      = batch_size;

        t.width     = renderer->width;
        t.height    = renderer->height;

        t.mesh      = mesh;
        t.camera    = camera;

        if (i == THREAD_COUNT - 1)
        {
            t.size = (mesh->indices_size / 3) - t.offset;
        }

        renderer->thread_data[i] = t;
        thread_pool_add_job(renderer->thread_pool, renderer_process_triangles, &renderer->thread_data[i]);
    }

    thread_pool_wait(renderer->thread_pool);

    triangle_t* triangles   = mesh->triangles;
    uint32_t triangles_size = mesh->triangles_size;

    for (uint32_t i = 0; i < triangles_size; i++)
    {
        rasterize_triangle(triangles[i].v0,
                           triangles[i].v1,
                           triangles[i].v2,
                           triangles[i].color,
                           renderer->current,
                           renderer->depthbuffer);
    }
}

/********************/
/* public functions */
/********************/

renderer_t* renderer_new(uint32_t width, uint32_t height, const char* file_path)
{

    renderer_t* renderer = malloc(sizeof(renderer_t));

    renderer->width         = width;
    renderer->height        = height;

    renderer->scene         = scene_new(file_path);
    renderer->display       = display_new(width, height);

    renderer->front         = framebuffer_new(width, height);
    renderer->back          = framebuffer_new(width, height);
    renderer->current       = renderer->front;

    renderer->depthbuffer   = depthbuffer_new(width, height);

    renderer->wireframe     = false;

    renderer->thread_pool   = thread_pool_new("Triangle Processor");
    memset(renderer->thread_data, 0, sizeof(triangle_batch_t) * THREAD_COUNT);

    return renderer;
}

void renderer_run(renderer_t* renderer)
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

        // process input
        input_t input = handle_input(renderer->display);

        // update
        scene_update(renderer->scene, input);
        if (input.keys & KEY_1)
        {
            renderer->wireframe = !renderer->wireframe;
        }

        // render
        render_utils(renderer);

        if (renderer->wireframe)
        {
            render_wireframe(renderer);
        }

        renderer_draw_mesh(renderer, renderer->scene->mesh);

        display_draw(renderer->display, renderer->current);

        // clear
        renderer_clear_buffers(renderer);

        // swap buffers
        renderer->current = renderer->current == renderer->front
                          ? renderer->back
                          : renderer->front;

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

void renderer_free(renderer_t* renderer)
{
    thread_pool_free(renderer->thread_pool);
    scene_free(renderer->scene);
    display_free(renderer->display);
    framebuffer_free(renderer->front);
    framebuffer_free(renderer->back);
    depthbuffer_free(renderer->depthbuffer);
    free(renderer);
}