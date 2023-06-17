#include "rasterizer.h"

#include <stdlib.h>
#include <stdbool.h>

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

/********************/
/* public functions */
/********************/

void rasterize_line(vec_t p1, 
                    vec_t p2, 
                    uint32_t color, 
                    frame_buffer_t* frame_buffer)
{
    int32_t x0 = (int32_t)p1.x;
    int32_t y0 = (int32_t)p1.y;
    int32_t x1 = (int32_t)p2.x;
    int32_t y1 = (int32_t)p2.y;
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
            frame_buffer_set(frame_buffer, (uint32_t)y, (uint32_t)x, color);
        }
        else
        {
            frame_buffer_set(frame_buffer, (uint32_t)x, (uint32_t)y, color);
        }

        error += de;
        if (error >= dx)
        {
            y += (y1 > y0) ? 1 : -1;
            error -= (dx << 1);
        }
    }

}

void rasterize_triangle()
{

}