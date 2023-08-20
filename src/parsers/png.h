#pragma once

#include <stddef.h>
#include "../texture.h"

#define MAX_TEXTURE_LOAD_COUNT 10

typedef struct
{
    uint32_t    size;
    texture_t*  textures[MAX_TEXTURE_LOAD_COUNT];
} texture_batch_t;

typedef struct
{
    uint32_t             size;
    uint32_t             buffer_sizes[MAX_TEXTURE_LOAD_COUNT];
    const unsigned char* buffers[MAX_TEXTURE_LOAD_COUNT];
} texture_batch_info_t;

texture_t*      parse_png(const unsigned char* buffer, uint32_t size);
texture_batch_t parse_multiple_pngs(texture_batch_info_t info);