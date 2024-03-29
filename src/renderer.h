#pragma once

#include <stdbool.h>

#include "scene.h"
#include "framebuffer.h"
#include "depthbuffer.h"
#include "linux/display.h"

typedef struct
{
    scene_t*        scene;
    display_t*      display;

    framebuffer_t*  current;
    framebuffer_t*  front;
    framebuffer_t*  back;

    depthbuffer_t*  depthbuffer;
    uint32_t        width;
    uint32_t        height;
    bool            wireframe;

} renderer_t;

renderer_t* renderer_new(uint32_t width, uint32_t height, const char* file_path);
void        renderer_run(renderer_t* renderer);
void        renderer_free(renderer_t* renderer);
