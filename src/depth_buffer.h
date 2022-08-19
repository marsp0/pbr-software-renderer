#pragma once

#include <stdint.h>

typedef struct
{
    int width;
    int height;
    float* data;

} depth_buffer_t;

depth_buffer_t* depth_buffer_new(int width, int height);
void depth_buffer_set(depth_buffer_t* buffer, int x, int y, float val);
float depth_buffer_get(depth_buffer_t* buffer, int x, int y);
void depth_buffer_free(depth_buffer_t* buffer);
