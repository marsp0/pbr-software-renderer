#include "rasterizer.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "shader.h"
#include "thread_pool.h"

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

typedef struct
{
    int32_t         min_x;
    int32_t         max_x;
    int32_t         min_y;
    int32_t         max_y;

    triangle_t*     triangle;

    float           inv_area;

    framebuffer_t*  framebuffer;
    depthbuffer_t*  depthbuffer;

} pixel_batch_t;

static thread_pool_t* thread_pool = NULL;
static pixel_batch_t  thread_data[MAX_JOBS];

/********************/
/* static functions */
/********************/

static int32_t edge_check(int32_t x0, int32_t y0, 
                          int32_t x1, int32_t y1, 
                          int32_t x2, int32_t y2)
{
    return (x2 - x0) * (y1 - y0) - (x1 - x0) * (y2 - y0);
}

static void rasterizer_process_pixels(void* args)
{
    pixel_batch_t* batch = (pixel_batch_t*)args;

    triangle_t* t   = batch->triangle;
    vec4_t v0       = t->v0;
    vec4_t v1       = t->v1;
    vec4_t v2       = t->v2;

    float inv_area  = batch->inv_area;

    int32_t x0      = (int32_t)v0.x;
    int32_t x1      = (int32_t)v1.x;
    int32_t x2      = (int32_t)v2.x;

    int32_t y0      = (int32_t)v0.y;
    int32_t y1      = (int32_t)v1.y;
    int32_t y2      = (int32_t)v2.y;

    int32_t min_x   = batch->min_x;
    int32_t max_x   = batch->max_x;
    int32_t min_y   = batch->min_y;
    int32_t max_y   = batch->max_y;

    framebuffer_t* framebuffer = batch->framebuffer;
    depthbuffer_t* depthbuffer = batch->depthbuffer;

    for (int32_t y = min_y; y <= max_y; y++)
    {
        for (int32_t x = min_x; x <= max_x; x++)
        {
            float w0 = (float)edge_check(x1, y1, x2, y2, x, y);
            float w1 = (float)edge_check(x2, y2, x0, y0, x, y);
            float w2 = (float)edge_check(x0, y0, x1, y1, x, y);

            if (w0 > 0 || w1 > 0 || w2 > 0)
            {
                continue;
            }

            // normalized barycentric coordinates
            w0 *= inv_area;
            w1 *= inv_area;
            w2 *= inv_area;

            // perspective correct interpolation of z
            float depth = w0 * v0.z + w1 * v1.z + w2 * v2.z;

            if (depth < depthbuffer_get(depthbuffer, (uint32_t)x, (uint32_t)y))
            {
                continue;
            }

            depthbuffer_set(depthbuffer, (uint32_t)x, (uint32_t)y, depth);
            
            uint32_t color = shader_process_fragment(t, w0, w1, w2);
            framebuffer_set(framebuffer, (uint32_t)x, (uint32_t)y, color);
        }
    }
}

/********************/
/* public functions */
/********************/

void rasterizer_init()
{
    thread_pool = thread_pool_new("Pixel Processor");
    memset(thread_data, 0, sizeof(pixel_batch_t) * THREAD_COUNT);
}

void rasterizer_draw_line(vec4_t v0,
                          vec4_t v1,
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

    int32_t dx      = x1 - x0;
    int32_t dy      = y1 - y0;
    int32_t px      = 0.f;
    int32_t py      = 0.f;
    int32_t de      = abs(dy) << 1;
    int32_t error   = 0;
    int32_t width   = (int32_t)framebuffer->width;
    int32_t height  = (int32_t)framebuffer->height;

    for (int32_t x = x0, y = y0; x <= x1; x++)
    {
        px = x;
        py = y;

        if (steep)
        {
            px = y;
            py = x;
        }

        if (px >= 0.f && px < width && py >= 0.f && py < height)
        {
            framebuffer_set(framebuffer, (uint32_t)px, (uint32_t)py, color);
        }

        error += de;
        if (error >= dx)
        {
            y += (y1 > y0) ? 1 : -1;
            error -= (dx << 1);
        }
    }

}

void rasterizer_draw_triangle(triangle_t* triangle,
                              framebuffer_t* framebuffer,
                              depthbuffer_t* depthbuffer)
{
    vec4_t v0 = triangle->v0;
    vec4_t v1 = triangle->v1;
    vec4_t v2 = triangle->v2;

    // workaround until clipping is implemented
    if (v0.z <= 0.f || v0.z > 1.f ||
        v1.z <= 0.f || v1.z > 1.f ||
        v2.z <= 0.f || v2.z > 1.f)
    {
        return;
    }

    int32_t x0 = (int32_t)v0.x;
    int32_t x1 = (int32_t)v1.x;
    int32_t x2 = (int32_t)v2.x;

    int32_t y0 = (int32_t)v0.y;
    int32_t y1 = (int32_t)v1.y;
    int32_t y2 = (int32_t)v2.y;

    int32_t width = (int32_t)framebuffer->width - 1;
    int32_t height = (int32_t)framebuffer->height - 1;
    
    // area of parallelogram
    float area = (float)edge_check(x0, y0, x1, y1, x2, y2);

    // NOTE: should this be epsilon compare?
    if (area == 0.f)
    {
        return;
    }

    float inv_area  = 1.f / area;

    // find min/max within buffer boundaries
    int32_t min_x = i_max(i_min(i_min(x0, x1), x2), 0.f);
    int32_t min_y = i_max(i_min(i_min(y0, y1), y2), 0.f);
    int32_t max_x = i_min(i_max(i_max(x0, x1), x2), width);
    int32_t max_y = i_min(i_max(i_max(y0, y1), y2), height);

    pixel_batch_t batch;
    int32_t job_id      = 0;
    int32_t submitted   = 0;
    int32_t total       = max_x - min_x;
    int32_t batch_size  = total > THREAD_COUNT ? total / THREAD_COUNT : total;

    while (submitted < total)
    {
        batch.min_x         = min_x + batch_size * job_id;
        batch.max_x         = min_x + batch_size * job_id + batch_size;
        batch.min_y         = min_y;
        batch.max_y         = max_y;
        batch.triangle      = triangle;
        batch.inv_area      = inv_area;
        batch.framebuffer   = framebuffer;
        batch.depthbuffer   = depthbuffer;

        thread_data[job_id] = batch;
        thread_pool_add(thread_pool, rasterizer_process_pixels, &thread_data[job_id]);

        job_id++;
        submitted += batch_size;
    }

    thread_pool_wait(thread_pool);
}

void rasterizer_free()
{
    thread_pool_free(thread_pool);
}