#pragma once

typedef enum
{
    POINT_SAMPLE = 0,
    BILINEAR_SAMPLE,
    TEXTURE_FILTER_SIZE
} texture_filter_e;

texture_filter_e    get_texture_filter();
void                change_texture_filter();