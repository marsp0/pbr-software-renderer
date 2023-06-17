#pragma once

#include <stdint.h>

typedef struct
{
    uint32_t        width;
    uint32_t        height;
    unsigned char*  data;       // BGRA

} framebuffer_t;

framebuffer_t*  framebuffer_new(uint32_t width, uint32_t height);
void            framebuffer_set(framebuffer_t* buffer, uint32_t x, uint32_t y, uint32_t color);
void            framebuffer_clear(framebuffer_t* buffer);
void            framebuffer_free(framebuffer_t* buffer);