#include "renderer.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

renderer_t* renderer_new(uint32_t width, uint32_t height, const char* file_path)
{

    renderer_t* renderer = malloc(sizeof(renderer_t));
    renderer->width = width;
    renderer->height = height;
    renderer->scene = scene_new(file_path);

    /* 
     * TODO 
     * allocate framebuffer
     * allocate depthbuffer
     */
    return renderer;
}

void renderer_run(renderer_t* renderer)
{
    bool shutdown = false;

    printf("%p\n", (void*)renderer);

    while (!shutdown)
    {
        // handle input

        // update

        // draw
        exit(1);
    }
}

// void renderer_render_mesh(renderer_t* renderer)
// {

// }

// void renderer_clear(renderer_t* renderer)
// {

// }

void renderer_free(renderer_t* renderer)
{
    /*free framebuffer*/
    /*free deothbuffer*/
    free(renderer);
}