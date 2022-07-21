#pragma once

#include <stdint.h>

typedef struct
{
    int width;
    int height;
    float* data;

} depthbuffer;

depthbuffer* depthbuffer_new(int width, int height);
void depthbuffer_set(depthbuffer* buffer, int x, int y, float val);
float depthbuffer_get(depthbuffer* buffer, int x, int y);
void depthbuffer_free(depthbuffer* buffer);
