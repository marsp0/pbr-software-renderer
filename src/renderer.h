#pragma once

#include "frame_buffer.h"
#include "depth_buffer.h"

typedef struct
{
    int width;
    int height;
    frame_buffer_t* frame_buffer;
    depth_buffer_t* depth_buffer;

} renderer_t;

renderer_t* renderer_new(int width, int height);
void renderer_run(renderer_t* renderer);
void renderer_draw(renderer_t* renderer);
void renderer_draw_model(renderer_t* renderer);
void renderer_clear(renderer_t* renderer);
void renderer_free(renderer_t* renderer);
