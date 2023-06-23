#include "test_rasterizer.h"

#include <stdio.h>

#include "test_utils.h"

#include "../rasterizer.h"
#include "../rasterizer_constants.h"

static void test_rasterize_line_horizontal()
{
    // +--------------------+
    // |                    |
    // |                    |
    // |                    |
    // |                    |
    // |                    |
    // |                    |
    // |                    |
    // |                    |
    // |  xxxxxxxxxxxx      |
    // |                    |
    // +--------------------+

    uint32_t width = 10;
    uint32_t height = 10;
    framebuffer_t* buffer = framebuffer_new(width, height);
    vec_t p1        = vec_new(1.f, 1.f, 0.f);
    vec_t p2        = vec_new(6.f, 1.f, 0.f);
    uint32_t color  = 0xAABBCCFF;

    for (uint32_t i = 0; i < width; i++)
    {
        for (uint32_t j = 0; j < height; j++)
        {
            ASSERT_EQUAL(0, framebuffer_get(buffer, i, j));
        }
    }

    // draw line
    rasterize_line(p1, p2, color, buffer);

    // assert pixel count that has changed
    uint32_t actual_count = 0;
    uint32_t expected_count = 24; // 6 pixels * 4 channels

    for (uint32_t i = 0; i < width * height * RGB_CHANNELS; i++)
    {
        if (buffer->data[i] != 0)
        {
            actual_count++;
        }
    }

    ASSERT_EQUAL(expected_count, actual_count);

    // assert individual pixels
    vec_t points[6] = {vec_new(1.f, 1.f, 0.f),
                       vec_new(2.f, 1.f, 0.f),
                       vec_new(3.f, 1.f, 0.f),
                       vec_new(4.f, 1.f, 0.f),
                       vec_new(5.f, 1.f, 0.f),
                       vec_new(6.f, 1.f, 0.f)};
    
    for (uint32_t i = 0; i < sizeof(points) / sizeof(vec_t); i++)
    {
        vec_t p = points[i];
        ASSERT_EQUAL(color, framebuffer_get(buffer, (uint32_t)p.x, (uint32_t)p.y));
    }

    framebuffer_free(buffer);
}

static void test_rasterize_line_vertical()
{
    // +--------------------+
    // |                    |
    // |                    |
    // |                    |
    // |  xx                |
    // |  xx                |
    // |  xx                |
    // |  xx                |
    // |  xx                |
    // |  xx                |
    // |                    |
    // +--------------------+

    uint32_t width = 10;
    uint32_t height = 10;
    framebuffer_t* buffer = framebuffer_new(width, height);
    vec_t p1        = vec_new(1.f, 1.f, 0.f);
    vec_t p2        = vec_new(1.f, 6.f, 0.f);
    uint32_t color  = 0xAABBCCFF;

    for (uint32_t i = 0; i < width; i++)
    {
        for (uint32_t j = 0; j < height; j++)
        {
            ASSERT_EQUAL(0, framebuffer_get(buffer, i, j));
        }
    }

    // draw line
    rasterize_line(p1, p2, color, buffer);

    // assert pixel count that has changed
    uint32_t actual_count = 0;
    uint32_t expected_count = 24; // 6 pixels * 4 channels

    for (uint32_t i = 0; i < width * height * RGB_CHANNELS; i++)
    {
        if (buffer->data[i] != 0)
        {
            actual_count++;
        }
    }

    ASSERT_EQUAL(expected_count, actual_count);

    // assert individual pixels
    vec_t points[6] = {vec_new(1.f, 1.f, 0.f),
                       vec_new(1.f, 2.f, 0.f),
                       vec_new(1.f, 3.f, 0.f),
                       vec_new(1.f, 4.f, 0.f),
                       vec_new(1.f, 5.f, 0.f),
                       vec_new(1.f, 6.f, 0.f)};
    
    for (uint32_t i = 0; i < sizeof(points) / sizeof(vec_t); i++)
    {
        vec_t p = points[i];
        ASSERT_EQUAL(color, framebuffer_get(buffer, (uint32_t)p.x, (uint32_t)p.y));
    }

    framebuffer_free(buffer);
}

static void test_rasterize_line_steep_pos_slope()
{
    // +--------------------+
    // |                    |
    // |                    |
    // |                    |
    // |                    |
    // |      xx            |
    // |      xx            |
    // |    xx              |
    // |    xx              |
    // |  xx                |
    // |                    |
    // +--------------------+

    uint32_t width = 10;
    uint32_t height = 10;
    framebuffer_t* buffer = framebuffer_new(width, height);
    vec_t p1        = vec_new(1.f, 1.f, 0.f);
    vec_t p2        = vec_new(3.f, 5.f, 0.f);
    uint32_t color  = 0xAABBCCFF;

    for (uint32_t i = 0; i < width; i++)
    {
        for (uint32_t j = 0; j < height; j++)
        {
            ASSERT_EQUAL(0, framebuffer_get(buffer, i, j));
        }
    }

    // draw line
    rasterize_line(p1, p2, color, buffer);

    // assert pixel count that has changed
    uint32_t actual_count = 0;
    uint32_t expected_count = 20; // 5 pixels * 4 channels

    for (uint32_t i = 0; i < width * height * RGB_CHANNELS; i++)
    {
        if (buffer->data[i] != 0)
        {
            actual_count++;
        }
    }

    ASSERT_EQUAL(expected_count, actual_count);

    // assert individual pixels
    vec_t points[5] = {vec_new(1.f, 1.f, 0.f),
                       vec_new(2.f, 2.f, 0.f),
                       vec_new(2.f, 3.f, 0.f),
                       vec_new(3.f, 4.f, 0.f),
                       vec_new(3.f, 5.f, 0.f)};
    
    for (uint32_t i = 0; i < sizeof(points) / sizeof(vec_t); i++)
    {
        vec_t p = points[i];
        ASSERT_EQUAL(color, framebuffer_get(buffer, (uint32_t)p.x, (uint32_t)p.y));
    }

    framebuffer_free(buffer);
}

static void test_rasterize_line_steep_neg_slope()
{
    // +--------------------+
    // |                    |
    // |                    |
    // |                    |
    // |                    |
    // |  xx                |
    // |  xx                |
    // |    xx              |
    // |    xx              |
    // |      xx            |
    // |                    |
    // +--------------------+

    uint32_t width = 10;
    uint32_t height = 10;
    framebuffer_t* buffer = framebuffer_new(width, height);
    vec_t p1        = vec_new(1.f, 5.f, 0.f);
    vec_t p2        = vec_new(3.f, 1.f, 0.f);
    uint32_t color  = 0xAABBCCFF;

    for (uint32_t i = 0; i < width; i++)
    {
        for (uint32_t j = 0; j < height; j++)
        {
            ASSERT_EQUAL(0, framebuffer_get(buffer, i, j));
        }
    }

    // draw line
    rasterize_line(p1, p2, color, buffer);

    // assert pixel count that has changed
    uint32_t actual_count = 0;
    uint32_t expected_count = 20; // 5 pixels * 4 channels

    for (uint32_t i = 0; i < width * height * RGB_CHANNELS; i++)
    {
        if (buffer->data[i] != 0)
        {
            actual_count++;
        }
    }

    ASSERT_EQUAL(expected_count, actual_count);

    // assert individual pixels
    vec_t points[5] = {vec_new(1.f, 5.f, 0.f),
                       vec_new(1.f, 4.f, 0.f),
                       vec_new(2.f, 3.f, 0.f),
                       vec_new(2.f, 2.f, 0.f),
                       vec_new(3.f, 1.f, 0.f)};
    
    for (uint32_t i = 0; i < sizeof(points) / sizeof(vec_t); i++)
    {
        vec_t p = points[i];
        ASSERT_EQUAL(color, framebuffer_get(buffer, (uint32_t)p.x, (uint32_t)p.y));
    }

    framebuffer_free(buffer);
}

static void test_rasterize_line_pos_slope()
{
    // +--------------------+
    // |                    |
    // |                    |
    // |                    |
    // |                    |
    // |                    |
    // |          xx        |
    // |      xxxx          |
    // |    xx              |
    // |  xx                |
    // |                    |
    // +--------------------+

    uint32_t width = 10;
    uint32_t height = 10;
    framebuffer_t* buffer = framebuffer_new(width, height);
    vec_t p1        = vec_new(1.f, 1.f, 0.f);
    vec_t p2        = vec_new(5.f, 4.f, 0.f);
    uint32_t color  = 0xAABBCCFF;

    for (uint32_t i = 0; i < width; i++)
    {
        for (uint32_t j = 0; j < height; j++)
        {
            ASSERT_EQUAL(0, framebuffer_get(buffer, i, j));
        }
    }

    // draw line
    rasterize_line(p1, p2, color, buffer);

    // assert pixel count that has changed
    uint32_t actual_count = 0;
    uint32_t expected_count = 20; // 5 pixels * 4 channels

    for (uint32_t i = 0; i < width * height * RGB_CHANNELS; i++)
    {
        if (buffer->data[i] != 0)
        {
            actual_count++;
        }
    }

    ASSERT_EQUAL(expected_count, actual_count);

    // assert individual pixels
    vec_t points[5] = {vec_new(1.f, 1.f, 0.f),
                       vec_new(2.f, 2.f, 0.f),
                       vec_new(3.f, 3.f, 0.f),
                       vec_new(4.f, 3.f, 0.f),
                       vec_new(5.f, 4.f, 0.f)};
    
    for (uint32_t i = 0; i < sizeof(points) / sizeof(vec_t); i++)
    {
        vec_t p = points[i];
        ASSERT_EQUAL(color, framebuffer_get(buffer, (uint32_t)p.x, (uint32_t)p.y));
    }

    framebuffer_free(buffer);
}

static void test_rasterize_line_neg_slope()
{
    // +--------------------+
    // |                    |
    // |                    |
    // |                    |
    // |                    |
    // |                    |
    // |                    |
    // |  xxxx              |
    // |      xxxx          |
    // |          xxxx      |
    // |                    |
    // +--------------------+

    uint32_t width = 10;
    uint32_t height = 10;
    framebuffer_t* buffer = framebuffer_new(width, height);
    vec_t p1        = vec_new(1.f, 3.f, 0.f);
    vec_t p2        = vec_new(6.f, 1.f, 0.f);
    uint32_t color  = 0xAABBCCFF;

    for (uint32_t i = 0; i < width; i++)
    {
        for (uint32_t j = 0; j < height; j++)
        {
            ASSERT_EQUAL(0, framebuffer_get(buffer, i, j));
        }
    }

    // draw line
    rasterize_line(p1, p2, color, buffer);

    // assert pixel count that has changed
    uint32_t actual_count = 0;
    uint32_t expected_count = 24; // 6 pixels * 4 channels

    for (uint32_t i = 0; i < width * height * RGB_CHANNELS; i++)
    {
        if (buffer->data[i] != 0)
        {
            actual_count++;
        }
    }

    ASSERT_EQUAL(expected_count, actual_count);

    // assert individual pixels
    vec_t points[6] = {vec_new(1.f, 3.f, 0.f),
                       vec_new(2.f, 3.f, 0.f),
                       vec_new(3.f, 2.f, 0.f),
                       vec_new(4.f, 2.f, 0.f),
                       vec_new(5.f, 1.f, 0.f),
                       vec_new(6.f, 1.f, 0.f)};
    
    for (uint32_t i = 0; i < sizeof(points) / sizeof(vec_t); i++)
    {
        vec_t p = points[i];
        ASSERT_EQUAL(color, framebuffer_get(buffer, (uint32_t)p.x, (uint32_t)p.y));
    }

    framebuffer_free(buffer);
}

static void test_rasterize_triangle_colinear_horizontal()
{
    // +--------------------+
    // |                    |
    // |                    |
    // |                    |
    // |                    |
    // |                    |
    // |                    |
    // |                    |
    // |                    |
    // |  xxxxxxxxxxxx      |
    // |                    |
    // +--------------------+

    uint32_t width = 10;
    uint32_t height = 10;
    framebuffer_t* framebuffer = framebuffer_new(width, height);
    depthbuffer_t* depthbuffer = depthbuffer_new(width, height);
    vec_t p1        = vec_new(1.f, 1.f, 0.f);
    vec_t p2        = vec_new(3.f, 1.f, 0.f);
    vec_t p3        = vec_new(6.f, 1.f, 0.f);
    uint32_t color  = 0xAABBCCFF;

    // assert initial values
    for (uint32_t i = 0; i < width; i++)
    {
        for (uint32_t j = 0; j < height; j++)
        {
            ASSERT_EQUAL(0, framebuffer_get(framebuffer, i, j));
            ASSERT_EQUAL(MAX_DEPTH, depthbuffer_get(depthbuffer, i, j));
        }
    }

    rasterize_triangle(p1, p2, p3, color, framebuffer, depthbuffer);

    // assert pixel count that has changed
    uint32_t actual_count = 0;
    uint32_t expected_count = 24; // 6 pixels * 4 channels

    for (uint32_t i = 0; i < width * height * RGB_CHANNELS; i++)
    {
        if (framebuffer->data[i] != 0)
        {
            actual_count++;
        }
    }

    ASSERT_EQUAL(expected_count, actual_count);

    // assert individual pixels
    vec_t points[6] = {vec_new(1.f, 1.f, 0.f),
                       vec_new(2.f, 1.f, 0.f),
                       vec_new(3.f, 1.f, 0.f),
                       vec_new(4.f, 1.f, 0.f),
                       vec_new(5.f, 1.f, 0.f),
                       vec_new(6.f, 1.f, 0.f)};
    
    for (uint32_t i = 0; i < sizeof(points) / sizeof(vec_t); i++)
    {
        vec_t p = points[i];
        ASSERT_EQUAL(color, framebuffer_get(framebuffer, (uint32_t)p.x, (uint32_t)p.y));
    }

    framebuffer_free(framebuffer);
    depthbuffer_free(depthbuffer);
}

static void test_rasterize_triangle_colinear_vertical()
{
    // +--------------------+
    // |                    |
    // |                    |
    // |                    |
    // |  xx                |
    // |  xx                |
    // |  xx                |
    // |  xx                |
    // |  xx                |
    // |  xx                |
    // |                    |
    // +--------------------+

    uint32_t width = 10;
    uint32_t height = 10;
    framebuffer_t* framebuffer = framebuffer_new(width, height);
    depthbuffer_t* depthbuffer = depthbuffer_new(width, height);
    vec_t p1        = vec_new(1.f, 1.f, 0.f);
    vec_t p2        = vec_new(1.f, 3.f, 0.f);
    vec_t p3        = vec_new(1.f, 6.f, 0.f);
    uint32_t color  = 0xAABBCCFF;

    // assert initial values
    for (uint32_t i = 0; i < width; i++)
    {
        for (uint32_t j = 0; j < height; j++)
        {
            ASSERT_EQUAL(0, framebuffer_get(framebuffer, i, j));
            ASSERT_EQUAL(MAX_DEPTH, depthbuffer_get(depthbuffer, i, j));
        }
    }

    rasterize_triangle(p1, p2, p3, color, framebuffer, depthbuffer);

    // assert pixel count that has changed
    uint32_t actual_count = 0;
    uint32_t expected_count = 24; // 6 pixels * 4 channels

    for (uint32_t i = 0; i < width * height * RGB_CHANNELS; i++)
    {
        if (framebuffer->data[i] != 0)
        {
            actual_count++;
        }
    }

    ASSERT_EQUAL(expected_count, actual_count);

    // assert individual pixels
    vec_t points[6] = {vec_new(1.f, 1.f, 0.f),
                       vec_new(1.f, 2.f, 0.f),
                       vec_new(1.f, 3.f, 0.f),
                       vec_new(1.f, 4.f, 0.f),
                       vec_new(1.f, 5.f, 0.f),
                       vec_new(1.f, 6.f, 0.f)};
    
    for (uint32_t i = 0; i < sizeof(points) / sizeof(vec_t); i++)
    {
        vec_t p = points[i];
        ASSERT_EQUAL(color, framebuffer_get(framebuffer, (uint32_t)p.x, (uint32_t)p.y));
    }

    framebuffer_free(framebuffer);
    depthbuffer_free(depthbuffer);
}

static void test_rasterize_triangle()
{
    // +--------------------+
    // /                    /
    // /                    /
    // /                    /
    // /      xx            /
    // /      xxxx          /
    // /      xxxxxx        /
    // /    xxxxxxxxxx      /
    // /    xxxxxxxxxxxx    /
    // /  xxxxxxxxxxxxxxxx  /
    // /                    /
    // +--------------------+

    uint32_t width = 10;
    uint32_t height = 10;
    framebuffer_t* framebuffer = framebuffer_new(width, height);
    depthbuffer_t* depthbuffer = depthbuffer_new(width, height);
    vec_t p1        = vec_new(1.f, 1.f, 0.f);
    vec_t p2        = vec_new(8.f, 1.f, 0.f);
    vec_t p3        = vec_new(3.f, 6.f, 0.f);
    uint32_t color  = 0xAABBCCFF;

    // assert initial values
    for (uint32_t i = 0; i < width; i++)
    {
        for (uint32_t j = 0; j < height; j++)
        {
            ASSERT_EQUAL(0, framebuffer_get(framebuffer, i, j));
            ASSERT_EQUAL(MAX_DEPTH, depthbuffer_get(depthbuffer, i, j));
        }
    }

    rasterize_triangle(p1, p2, p3, color, framebuffer, depthbuffer);

    

    // assert pixel count that has changed
    uint32_t actual_count = 0;
    uint32_t expected_count = 100; // 25 pixels * 4 channels

    for (uint32_t i = 0; i < width * height * RGB_CHANNELS; i++)
    {
        if (framebuffer->data[i] != 0)
        {
            actual_count++;
        }
    }

    ASSERT_EQUAL(expected_count, actual_count);

    vec_t points[25] = {vec_new(1.f, 1.f, 1.f),
                        vec_new(2.f, 1.f, 1.f),
                        vec_new(3.f, 1.f, 1.f),
                        vec_new(4.f, 1.f, 1.f),
                        vec_new(5.f, 1.f, 1.f),
                        vec_new(6.f, 1.f, 1.f),
                        vec_new(7.f, 1.f, 1.f),
                        vec_new(8.f, 1.f, 1.f),
                        vec_new(2.f, 2.f, 1.f),
                        vec_new(3.f, 2.f, 1.f),
                        vec_new(4.f, 2.f, 1.f),
                        vec_new(5.f, 2.f, 1.f),
                        vec_new(6.f, 2.f, 1.f),
                        vec_new(7.f, 2.f, 1.f),
                        vec_new(2.f, 3.f, 1.f),
                        vec_new(3.f, 3.f, 1.f),
                        vec_new(4.f, 3.f, 1.f),
                        vec_new(5.f, 3.f, 1.f),
                        vec_new(6.f, 3.f, 1.f),
                        vec_new(3.f, 4.f, 1.f),
                        vec_new(4.f, 4.f, 1.f),
                        vec_new(5.f, 4.f, 1.f),
                        vec_new(3.f, 5.f, 1.f),
                        vec_new(4.f, 5.f, 1.f),
                        vec_new(3.f, 6.f, 1.f)};

    for (uint32_t i = 0; i < sizeof(points) / sizeof(vec_t); i++)
    {
        vec_t p = points[i];
        ASSERT_EQUAL(color, framebuffer_get(framebuffer, (uint32_t)p.x, (uint32_t)p.y));
    }

    framebuffer_free(framebuffer);
    depthbuffer_free(depthbuffer);
}

static void test_rasterize_multiple_triangles()
{
    // +--------------------+
    // /                    /
    // /                    /
    // /                    /
    // /      xx    xx      /
    // /      xxxx  xx      /
    // /      xxxxxxxx      /
    // /    xxxxxxxxxxxx    /
    // /    xxxxxxxxxxxx    /
    // /  xxxxxxxxxxxxxxxx  /
    // /                    /
    // +--------------------+

    uint32_t width = 10;
    uint32_t height = 10;
    framebuffer_t* framebuffer = framebuffer_new(width, height);
    depthbuffer_t* depthbuffer = depthbuffer_new(width, height);
    vec_t p1        = vec_new(1.f, 1.f, 2.f);
    vec_t p2        = vec_new(8.f, 1.f, 2.f);
    vec_t p3        = vec_new(3.f, 6.f, 2.f);
    uint32_t color1 = 0xAABBCCFF;

    // assert initial values
    for (uint32_t i = 0; i < width; i++)
    {
        for (uint32_t j = 0; j < height; j++)
        {
            ASSERT_EQUAL(0, framebuffer_get(framebuffer, i, j));
            ASSERT_EQUAL(MAX_DEPTH, depthbuffer_get(depthbuffer, i, j));
        }
    }

    // rasterize first triangle
    rasterize_triangle(p1, p2, p3, color1, framebuffer, depthbuffer);

    p1 = vec_new(6.f, 6.f, 1.f);
    p2 = vec_new(6.f, 1.f, 1.f);
    p3 = vec_new(8.f, 1.f, 1.f);
    uint32_t color2 = 0x11223344;

    // rasterize second triangle
    rasterize_triangle(p1, p2, p3, color2, framebuffer, depthbuffer);

    // assert pixel count that has changed
    uint32_t actual_count = 0;
    uint32_t expected_count = 116; // 29 pixels * 4 channels

    for (uint32_t i = 0; i < width * height * RGB_CHANNELS; i++)
    {
        if (framebuffer->data[i] != 0)
        {
            actual_count++;
        }
    }

    ASSERT_EQUAL(expected_count, actual_count);

    // assert first triangle
    vec_t points[19] = {vec_new(1.f, 1.f, 1.f),
                        vec_new(2.f, 1.f, 1.f),
                        vec_new(3.f, 1.f, 1.f),
                        vec_new(4.f, 1.f, 1.f),
                        vec_new(5.f, 1.f, 1.f),
                        vec_new(2.f, 2.f, 1.f),
                        vec_new(3.f, 2.f, 1.f),
                        vec_new(4.f, 2.f, 1.f),
                        vec_new(5.f, 2.f, 1.f),
                        vec_new(2.f, 3.f, 1.f),
                        vec_new(3.f, 3.f, 1.f),
                        vec_new(4.f, 3.f, 1.f),
                        vec_new(5.f, 3.f, 1.f),
                        vec_new(3.f, 4.f, 1.f),
                        vec_new(4.f, 4.f, 1.f),
                        vec_new(5.f, 4.f, 1.f),
                        vec_new(3.f, 5.f, 1.f),
                        vec_new(4.f, 5.f, 1.f),
                        vec_new(3.f, 6.f, 1.f)};

    for (uint32_t i = 0; i < sizeof(points) / sizeof(vec_t); i++)
    {
        vec_t p = points[i];
        ASSERT_EQUAL(color1, framebuffer_get(framebuffer, (uint32_t)p.x, (uint32_t)p.y));
    }

    // assert second triangle
    vec_t points2[10] = {vec_new(6.f, 1.f, 1.f),
                         vec_new(7.f, 1.f, 1.f),
                         vec_new(8.f, 1.f, 1.f),
                         vec_new(6.f, 2.f, 1.f),
                         vec_new(7.f, 2.f, 1.f),
                         vec_new(6.f, 3.f, 1.f),
                         vec_new(7.f, 3.f, 1.f),
                         vec_new(6.f, 4.f, 1.f),
                         vec_new(6.f, 5.f, 1.f),
                         vec_new(6.f, 6.f, 1.f)};

    for (uint32_t i = 0; i < sizeof(points2) / sizeof(vec_t); i++)
    {
        vec_t p = points2[i];
        ASSERT_EQUAL(color2, framebuffer_get(framebuffer, (uint32_t)p.x, (uint32_t)p.y));
    }

    framebuffer_free(framebuffer);
    depthbuffer_free(depthbuffer);
}

void test_rasterizer()
{
    // printf("+--------------------+\n");
    // for (int i = 0; i < (int)width; i++)
    // {
    //     printf("/");
    //     for (int j = 0; j < (int)height; j++)
    //     {
    //         int index = (i * (int)width + j) * RGB_CHANNELS;

    //         if (framebuffer->data[index] != 0)
    //         {
    //             printf("xx");
    //         }
    //         else
    //         {
    //             printf("  ");
    //         }
    //     }
    //     printf("/\n");
    // }
    // printf("+--------------------+\n");
    TEST_CASE(test_rasterize_line_horizontal);
    TEST_CASE(test_rasterize_line_vertical);
    TEST_CASE(test_rasterize_line_steep_pos_slope);
    TEST_CASE(test_rasterize_line_steep_neg_slope);
    TEST_CASE(test_rasterize_line_pos_slope);
    TEST_CASE(test_rasterize_line_neg_slope);
    TEST_CASE(test_rasterize_triangle_colinear_horizontal);
    TEST_CASE(test_rasterize_triangle_colinear_vertical);
    TEST_CASE(test_rasterize_triangle);
    TEST_CASE(test_rasterize_multiple_triangles);
}