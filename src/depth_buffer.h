#pragma once

#include <stdint.h>

typedef struct
{
    uint16_t width;
    uint16_t height;
    float* data;

} depthbuffer;

depthbuffer* depthbuffer_new(uint16_t width, uint16_t height);
void depthbuffer_set(depthbuffer* buffer, int x, int y, float val);
float depthbuffer_get(depthbuffer* buffer, int x, int y);
void depthbuffer_free(depthbuffer* buffer);
