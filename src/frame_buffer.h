#pragma once

#include <stdint.h>

#include "color.h"

typedef struct framebuffer
{
    int width;
    int height;
    color* data;

} framebuffer;

framebuffer* framebuffer_new(int width, int height);
void framebuffer_set(framebuffer* buffer, int x, int y, color val);
color framebuffer_get(framebuffer* buffer, int x, int y);
void framebuffer_destroy(framebuffer* buffer);