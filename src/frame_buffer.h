#pragma once

#include <stdint.h>

#include "color.h"

typedef struct
{
    int width;
    int height;
    uint32_t* data;

} frame_buffer_t;

frame_buffer_t* frame_buffer_new(int width, int height);
void frame_buffer_set(frame_buffer_t* buffer, int x, int y, color val);
uint32_t frame_buffer_get(frame_buffer_t* buffer, int x, int y);
void frame_buffer_free(frame_buffer_t* buffer);