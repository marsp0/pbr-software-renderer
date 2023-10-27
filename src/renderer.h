#pragma once

#include <stdbool.h>

#include "scene.h"
#include "framebuffer.h"
#include "depthbuffer.h"
#include "thread_pool.h"
#include "linux/display.h"

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

typedef struct
{
    uint32_t                width;
    uint32_t                height;

    scene_t*                scene;
    display_t*              display;

    framebuffer_t*          current;
    framebuffer_t*          front;
    framebuffer_t*          back;

    depthbuffer_t*          depthbuffer;

    bool                    wireframe;

    thread_pool_t*          thread_pool;
    triangle_batch_t        thread_data[THREAD_COUNT];

} renderer_t;

renderer_t* renderer_new(uint32_t width, uint32_t height, const char* file_path);
void        renderer_run(renderer_t* renderer);
void        renderer_free(renderer_t* renderer);
