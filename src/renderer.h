#pragma once

#include "scene.h"
#include "frame_buffer.h"
#include "depth_buffer.h"
#include "linux/display.h"

typedef struct
{
    scene_t*        scene;
    display_t*      display;

    frame_buffer_t* current;
    frame_buffer_t* front;
    frame_buffer_t* back;

    depth_buffer_t* depth_buffer;
    uint32_t        width;
    uint32_t        height;
} renderer_t;

renderer_t* renderer_new(uint32_t width, uint32_t height, const char* file_path);
void        renderer_run(renderer_t* renderer);
void        renderer_draw_mesh(renderer_t* renderer, mesh_t* mesh);
void        renderer_free(renderer_t* renderer);
