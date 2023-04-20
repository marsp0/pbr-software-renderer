#include "renderer.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "linux/input.h"
#include "time_utils.h"

renderer_t* renderer_new(uint32_t width, uint32_t height, const char* file_path)
{

    renderer_t* renderer = malloc(sizeof(renderer_t));

    renderer->scene         = scene_new(file_path);
    renderer->display       = display_new(width, height);

    renderer->front         = frame_buffer_new(width, height);
    renderer->back          = frame_buffer_new(width, height);
    renderer->current       = renderer->front;

    renderer->depth_buffer  = depth_buffer_new(width, height);

    renderer->width         = width;
    renderer->height        = height;

    return renderer;
}

void renderer_run(renderer_t* renderer)
{
    input_t input = 0;
    timestamp_t start;
    timestamp_t end;
    timestamp_t diff;
    timestamp_t frame_time = 16 * MILLISECOND;

    while (!(input & QUIT))
    {
        start = time_now();

        // process input
        handle_input(renderer->display, &input);

        // update

        // render
        // display_draw_mesh(renderer, renderer->mesh);
        display_draw(renderer->display, renderer->current);
        display_clear(renderer->display);
        renderer->current = renderer->current == renderer->front
                          ? renderer->back
                          : renderer->front;

        // maintain 60fps
        end = time_now();
        diff = end - start;

        if (frame_time > diff)
        {
            nanosleep((const struct timespec[]){{ 0, frame_time - diff }}, NULL);
        }
    }
}

void renderer_free(renderer_t* renderer)
{
    scene_free(renderer->scene);
    display_free(renderer->display);
    frame_buffer_free(renderer->front);
    frame_buffer_free(renderer->back);
    depth_buffer_free(renderer->depth_buffer);
    free(renderer);
}