#pragma once

#include <stdint.h>

#define MAX_DEPTH 10000.f

typedef struct
{
    uint32_t width;
    uint32_t height;
    uint32_t origin;
    float*   data;

} depthbuffer_t;

depthbuffer_t*  depthbuffer_new(uint32_t width, uint32_t height);
void            depthbuffer_set(depthbuffer_t* buffer, uint32_t x, uint32_t y, float val);
float           depthbuffer_get(depthbuffer_t* buffer, uint32_t x, uint32_t y);
void            depthbuffer_clear(depthbuffer_t* buffer);
void            depthbuffer_free(depthbuffer_t* buffer);
