#pragma once

#include <stdint.h>

#include "color.h"

typedef struct
{
    int width;
    int height;
    uint32_t* data;

} framebuffer;

framebuffer* framebuffer_new(int width, int height);
void framebuffer_set(framebuffer* buffer, int x, int y, color val);
uint32_t framebuffer_get(framebuffer* buffer, int x, int y);
void framebuffer_free(framebuffer* buffer);