#pragma once

#include "frame_buffer.h"
#include "depth_buffer.h"
#include "scene.h"

typedef struct
{
    scene_t*        scene;
    frame_buffer_t* frame_buffer;
    depth_buffer_t* depth_buffer;
    uint32_t        width;
    uint32_t        height;
} renderer_t;

renderer_t* renderer_new(uint32_t width, uint32_t height, const char* file_path);
void        renderer_run(renderer_t* renderer);
void        renderer_draw_mesh(renderer_t* renderer, mesh_t* mesh);
void        renderer_clear(renderer_t* renderer);
void        renderer_load_scene(renderer_t* renderer, const char* file_path);
void        renderer_free(renderer_t* renderer);
