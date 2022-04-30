#pragma once

static const float ONE_OVER_255 = 0.0039215;

typedef struct color
{
    float r;
    float g;
    float b;

} color;

color color_new_f(float r, float g, float b);
color color_new_u(unsigned char r, unsigned char g, unsigned char b);
color color_add(color c1, color c2);
color color_mul(color c1, color c2);
color color_scale(color col, float scale);
