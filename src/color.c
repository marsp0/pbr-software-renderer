#include "color.h"

color color_new_f(float r, float g, float b)
{
    color c;
    c.r = r;
    c.g = g;
    c.b = b;
    return c;
}

color color_new_u(unsigned char r, unsigned char g, unsigned char b)
{
    color c;
    c.r = r * ONE_OVER_255;
    c.g = g * ONE_OVER_255;
    c.b = b * ONE_OVER_255;
    return c;
}

color color_add(color c1, color c2)
{
    color c;
    c.r = c1.r + c2.r;
    c.g = c1.g + c2.g;
    c.b = c1.b + c2.b;
    return c;
}

color color_mul(color c1, color c2)
{
    color c;
    c.r = c1.r * c2.r;
    c.g = c1.g * c2.g;
    c.b = c1.b * c2.b;
    return c;
}

color color_scale(color col, float scale)
{
    color c;
    c.r = col.r * scale;
    c.g = col.g * scale;
    c.b = col.b * scale;
    return c;
}