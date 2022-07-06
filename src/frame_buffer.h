#pragma once

#include <stdint.h>

#include "color.h"

typedef struct
{
    uint16_t width;
    uint16_t height;
    uint32_t* data;

} framebuffer;

framebuffer* framebuffer_new(uint16_t width, uint16_t height);
void framebuffer_set(framebuffer* buffer, int x, int y, color val);
uint32_t framebuffer_get(framebuffer* buffer, int x, int y);
void framebuffer_free(framebuffer* buffer);