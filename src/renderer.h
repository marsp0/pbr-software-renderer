#pragma once

#include "frame_buffer.h"
#include "depth_buffer.h"

typedef struct
{
    int width;
    int height;
    framebuffer* framebuf;
    depthbuffer* depthbuf;

} renderer;

renderer* renderer_new(int width, int height);
void renderer_run(renderer* rendr);
void renderer_draw(renderer* rendr);
void renderer_draw_model(renderer* rendr);
void renderer_clear(renderer* rendr);
