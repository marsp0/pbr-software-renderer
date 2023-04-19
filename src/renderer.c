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
    renderer->frame_buffer  = frame_buffer_new(width, height);
    renderer->depth_buffer  = depth_buffer_new(width, height);
    renderer->width         = width;
    renderer->height        = height;

    return renderer;
}

void renderer_run(renderer_t* renderer)
{
    input_t input       = 0;
    // timepoint_t before  = {0, 0};
    // timepoint_t after   = {0, 0};

    printf("%p\n", (void*)renderer);

    while (!(input & QUIT))
    {
        printf("new frame\n");

        // process input
        handle_input(renderer->display, &input);

        // update

        // render
        // display_draw_mesh(renderer, renderer->mesh);
        display_draw(renderer->display, renderer->frame_buffer);
        display_clear(renderer->display);
    }
}

void renderer_free(renderer_t* renderer)
{
    scene_free(renderer->scene);
    display_free(renderer->display);
    frame_buffer_free(renderer->frame_buffer);
    depth_buffer_free(renderer->depth_buffer);
    free(renderer);
}