#include "rasterizer.h"

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/********************
 *  Notes
 *
 ********************/

/********************/
/*      defines     */
/********************/

/********************/
/* static variables */
/********************/

/********************/
/* static functions */
/********************/

static int32_t edge_check(int32_t x0, int32_t y0, 
                          int32_t x1, int32_t y1, 
                          int32_t x2, int32_t y2)
{
    return (x2 - x0) * (y1 - y0) - (x1 - x0) * (y2 - y0);
}

/********************/
/* public functions */
/********************/

void rasterize_line(vec_t v0,
                    vec_t v1,
                    uint32_t color, 
                    framebuffer_t* framebuffer)
{
    int32_t x0 = (int32_t)v0.x;
    int32_t y0 = (int32_t)v0.y;
    int32_t x1 = (int32_t)v1.x;
    int32_t y1 = (int32_t)v1.y;
    int32_t temp = x0;
    bool steep = false;

    // draw by iterating over the longer side
    if (abs(x1 - x0) < abs(y1 - y0))
    {
        temp = x0;
        x0   = y0;
        y0   = temp;

        temp = x1;
        x1   = y1;
        y1   = temp;

        steep = true;
    }

    // draw from right to left
    if (x0 > x1)
    {
        temp = x0;
        x0   = x1;
        x1   = temp;

        temp = y0;
        y0 = y1;
        y1 = temp;
    }

    int32_t dx = x1 - x0;
    int32_t dy = y1 - y0;
    int32_t de = abs(dy) << 1;
    int32_t error = 0;

    for (int32_t x = x0, y = y0; x <= x1; x++)
    {
        if (steep)
        {
            framebuffer_set(framebuffer, (uint32_t)y, (uint32_t)x, color);
        }
        else
        {
            framebuffer_set(framebuffer, (uint32_t)x, (uint32_t)y, color);
        }

        error += de;
        if (error >= dx)
        {
            y += (y1 > y0) ? 1 : -1;
            error -= (dx << 1);
        }
    }

}

void rasterize_triangle(vec_t v0,
                        vec_t v1,
                        vec_t v2,
                        uint32_t color,
                        framebuffer_t* framebuffer,
                        depthbuffer_t* depthbuffer)
{
    int32_t x0 = (int32_t)v0.x;
    int32_t x1 = (int32_t)v1.x;
    int32_t x2 = (int32_t)v2.x;

    int32_t y0 = (int32_t)v0.y;
    int32_t y1 = (int32_t)v1.y;
    int32_t y2 = (int32_t)v2.y;

    float inverse_z0 = 1.f/v0.z;
    float inverse_z1 = 1.f/v1.z;
    float inverse_z2 = 1.f/v2.z;

    // find min/max within buffer boundaries
    int32_t minx = (int32_t)fmax(fmin(fmin(x0, x1), x2), 0.f);
    int32_t miny = (int32_t)fmax(fmin(fmin(y0, y1), y2), 0.f);
    int32_t maxx = (int32_t)fmin(fmax(fmax(x0, x1), x2), framebuffer->width - 1);
    int32_t maxy = (int32_t)fmin(fmax(fmax(y0, y1), y2), framebuffer->height - 1);

    // area of parallelogram
    float inverse_area = 1.f / (float)edge_check(x0, y0, x1, y1, x2, y2);

    for (int32_t y = miny; y <= maxy; y++)
    {
        for (int32_t x = minx; x <= maxx; x++)
        {
            float w0 = (float)edge_check(x1, y1, x2, y2, x, y);
            float w1 = (float)edge_check(x2, y2, x0, y0, x, y);
            float w2 = (float)edge_check(x0, y0, x1, y1, x, y);

            if (w0 > 0 || w1 > 0 || w2 > 0)
            {
                continue;
            }

            // normalized barycentric coordinates
            w0 *= inverse_area;
            w1 *= inverse_area;
            w2 *= inverse_area;

            // perspective correct interpolation of z
            float depth = 1.f/(w0 * inverse_z0 + w1 * inverse_z1 + w2 * inverse_z2);

            if (depth > depthbuffer_get(depthbuffer, (uint32_t)x, (uint32_t)y))
            {
                continue;
            }

            depthbuffer_set(depthbuffer, (uint32_t)x, (uint32_t)y, depth);
            framebuffer_set(framebuffer, (uint32_t)x, (uint32_t)y, color);
        }
    }
}