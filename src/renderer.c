#include "renderer.h"

#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

static scene_t* scene               = NULL;
static display_t* display           = NULL;
static framebuffer_t* front         = NULL;
static framebuffer_t* back          = NULL;
static framebuffer_t* current       = NULL;
static depthbuffer_t* depthbuffer   = NULL;
static uint32_t width               = 800;
static uint32_t height              = 600;
static bool wireframe               = false;

/********************/
/* static functions */
/********************/

static void renderer_update(input_t input)
{
    scene_update(scene, input);
    if (input.keys & KEY_1)
    {
        wireframe = !wireframe;
    }
}

static void renderer_draw_utilities()
{
    float w         = (float)width;
    float h         = (float)height;
    camera_t* cam   = scene->camera;

    vec4_t points[4]    = {vec4_new(0.f, 0.f, 0.f),
                           vec4_new(1.f, 0.f, 0.f),
                           vec4_new(0.f, 1.f, 0.f),
                           vec4_new(0.f, 0.f, 1.f)};

    uint32_t colors[4] = {0x00000000, 
                          0x0000FF00, 
                          0x00FF0000, 
                          0xFF000000};

    mat_t PV = mat_mul_mat(camera_proj_transform(cam),
                           camera_view_transform(cam));

    for (uint32_t i = 0; i < sizeof(points) / sizeof(vec4_t); i++)
    {
        points[i]   = mat_mul_vec(PV, points[i]);
        points[i]   = vec4_scale(points[i], 1.f/points[i].w);
        points[i].x = (points[i].x + 1.f) * 0.5f * w;
        points[i].y = (points[i].y + 1.f) * 0.5f * h;
    }

    rasterizer_draw_line(points[0], points[1], colors[1], current);
    rasterizer_draw_line(points[0], points[2], colors[2], current);
    rasterizer_draw_line(points[0], points[3], colors[3], current);
}

static void renderer_draw_mesh(mesh_t* mesh)
{
    printf("mesh draw called %p\n", (void*)mesh);
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

void renderer_init(const uint32_t w, const uint32_t h)
{
    display       = display_new(w, h);

    front         = framebuffer_new(w, h);
    back          = framebuffer_new(w, h);
    current       = front;

    depthbuffer   = depthbuffer_new(w, h);

    width         = w;
    height        = h;
    wireframe     = false;
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
    if (scene)
    {
        scene_free(scene);
    }
    display_free(display);
    framebuffer_free(front);
    framebuffer_free(back);
    depthbuffer_free(depthbuffer);
}