#pragma once

#include <stdint.h>

typedef struct
{
    uint32_t width;
    uint32_t height;
    float*   data;

} depth_buffer_t;

depth_buffer_t* depth_buffer_new(uint32_t width, uint32_t height);
void depth_buffer_set(depth_buffer_t* buffer, uint32_t x, uint32_t y, float val);
float depth_buffer_get(depth_buffer_t* buffer, uint32_t x, uint32_t y);
void depth_buffer_free(depth_buffer_t* buffer);
