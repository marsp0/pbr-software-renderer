#pragma once

#include <stdint.h>

#include "color.h"

typedef struct
{
    uint32_t        width;
    uint32_t        height;
    unsigned char*  data;       // BGRA

} frame_buffer_t;

frame_buffer_t* frame_buffer_new(uint32_t width, uint32_t height);
void            frame_buffer_set(frame_buffer_t* buffer, uint32_t x, uint32_t y, color val);
unsigned char   frame_buffer_get(frame_buffer_t* buffer, uint32_t x, uint32_t y);
void            frame_buffer_free(frame_buffer_t* buffer);