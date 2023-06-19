#include "renderer.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "linux/input.h"
#include "time_utils.h"
#include "rasterizer.h"

renderer_t* renderer_new(uint32_t width, uint32_t height, const char* file_path)
{

    renderer_t* renderer = malloc(sizeof(renderer_t));

    renderer->scene         = scene_new(file_path);
    renderer->display       = display_new(width, height);

    renderer->front         = framebuffer_new(width, height);
    renderer->back          = framebuffer_new(width, height);
    renderer->current       = renderer->front;

    renderer->depthbuffer   = depthbuffer_new(width, height);

    renderer->width         = width;
    renderer->height        = height;

    return renderer;
}

// void renderer_draw_mesh(renderer_t* renderer, mesh_t* mesh)
// {

// }

void renderer_clear(renderer_t* renderer)
{
    framebuffer_clear(renderer->current);
    depthbuffer_clear(renderer->depthbuffer);
    display_clear(renderer->display);
}

void renderer_run(renderer_t* renderer)
{
    input_t input = 0;
    timestamp_t start;
    timestamp_t end;
    timestamp_t diff;
    timestamp_t frame_time = 16 * MILLISECOND;
    int counter = 0;
    int multiplier = 1;

    while (!(input & QUIT))
    {
        start = time_now();

        // process input
        handle_input(renderer->display, &input);

        // update
        // camera_update(renderer->scene->camera);

        // render
        rasterize_triangle(vec_new((float)multiplier * 50 + 100.f, 100.f, 50.f),
                           vec_new((float)multiplier * 50 + 100.f, 200.f, 50.f),
                           vec_new((float)multiplier * 50 + 100.f, 300.f, 50.f),
                           0xffffffff,
                           renderer->current,
                           renderer->depthbuffer);
        // display_draw_mesh(renderer, renderer->mesh);
        display_draw(renderer->display, renderer->current);
        renderer_clear(renderer);

        // swap buffers
        renderer->current = renderer->current == renderer->front
                          ? renderer->back
                          : renderer->front;

        // maintain 60fps
        end = time_now();
        diff = end - start;

        printf("%luus(%lu - %lu)\n", diff / 1000, frame_time, diff);

        if (frame_time > diff)
        {
            nanosleep((const struct timespec[]){{ 0, frame_time - diff }}, NULL);
        }

        counter++;
        if (counter == 60)
        {
            multiplier += 1;

            if (multiplier == 8)
            {
                multiplier = 0;
            }

            counter = 0;
        }
    }
}

void renderer_free(renderer_t* renderer)
{
    scene_free(renderer->scene);
    display_free(renderer->display);
    framebuffer_free(renderer->front);
    framebuffer_free(renderer->back);
    depthbuffer_free(renderer->depthbuffer);
    free(renderer);
}