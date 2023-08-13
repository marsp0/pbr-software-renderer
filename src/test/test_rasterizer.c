#include "test_rasterizer.h"

#include "test_utils.h"

#include "../rasterizer.h"
#include "../rasterizer_constants.h"

static void rasterize_and_assert(vec_t* input, 
                                 uint32_t input_size, 
                                 vec_t* output, 
                                 uint32_t output_size, 
                                 uint32_t color)
{
    uint32_t width = 10;
    uint32_t height = 10;
    framebuffer_t* buffer = framebuffer_new(width, height);
    depthbuffer_t* depthbuffer = depthbuffer_new(width, height);

    for (uint32_t i = 0; i < width; i++)
    {
        for (uint32_t j = 0; j < height; j++)
        {
            ASSERT_EQUAL(0, framebuffer_get(buffer, i, j));
            ASSERT_EQUAL(0.f, depthbuffer_get(depthbuffer, i, j));
        }
    }

    if (input_size == 2)
    {
        rasterize_line(input[0], input[1], color, buffer);
    }
    else if (input_size == 3)
    {
        rasterize_triangle(input[0], input[1], input[2], color, buffer, depthbuffer);
    }

    // assert pixel count that has changed
    uint32_t actual_count = 0;
    uint32_t expected_count = output_size * 4; // pixels * 4 channels

    for (uint32_t i = 0; i < width * height * RGB_CHANNELS; i++)
    {
        if (buffer->data[i] != 0)
        {
            actual_count++;
        }
    }

    ASSERT_EQUAL(expected_count, actual_count);

    // assert individual pixels    
    for (uint32_t i = 0; i < output_size; i++)
    {
        vec_t p = output[i];
        ASSERT_EQUAL(color, framebuffer_get(buffer, (uint32_t)p.x, (uint32_t)p.y));
    }

    framebuffer_free(buffer);
    depthbuffer_free(depthbuffer);
}

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

    vec_t input[2]  = {vec_new(1.f, 1.f, 0.f), 
                       vec_new(6.f, 1.f, 0.f)};
    vec_t output[6] = {vec_new(1.f, 1.f, 0.f),
                       vec_new(2.f, 1.f, 0.f),
                       vec_new(3.f, 1.f, 0.f),
                       vec_new(4.f, 1.f, 0.f),
                       vec_new(5.f, 1.f, 0.f),
                       vec_new(6.f, 1.f, 0.f)};

    rasterize_and_assert(input, 
                         sizeof(input) / sizeof(vec_t),
                         output,
                         sizeof(output) / sizeof(vec_t),
                         0xAABBCCFF);

}

static void test_rasterize_line_horizontal_out_of_bounds()
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
    // |xxxxxxxxxxxxxx      |
    // |                    |
    // +--------------------+

    vec_t input[2]  = {vec_new(-1.f, 1.f, 0.f), 
                       vec_new(6.f, 1.f, 0.f)};
    vec_t output[7] = {vec_new(0.f, 1.f, 0.f),
                       vec_new(1.f, 1.f, 0.f),
                       vec_new(2.f, 1.f, 0.f),
                       vec_new(3.f, 1.f, 0.f),
                       vec_new(4.f, 1.f, 0.f),
                       vec_new(5.f, 1.f, 0.f),
                       vec_new(6.f, 1.f, 0.f)};

    rasterize_and_assert(input, 
                         sizeof(input) / sizeof(vec_t),
                         output,
                         sizeof(output) / sizeof(vec_t),
                         0xAABBCCFF);
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

    vec_t input[2]  = {vec_new(1.f, 1.f, 0.f), 
                       vec_new(1.f, 6.f, 0.f)};
    vec_t output[6] = {vec_new(1.f, 1.f, 0.f),
                       vec_new(1.f, 2.f, 0.f),
                       vec_new(1.f, 3.f, 0.f),
                       vec_new(1.f, 4.f, 0.f),
                       vec_new(1.f, 5.f, 0.f),
                       vec_new(1.f, 6.f, 0.f)};

    rasterize_and_assert(input, 
                         sizeof(input) / sizeof(vec_t),
                         output,
                         sizeof(output) / sizeof(vec_t),
                         0xAABBCCFF);
}

static void test_rasterize_line_vertical_out_of_bounds()
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
    // |  xx                |
    // +--------------------+

    vec_t input[2]  = {vec_new(1.f, -1.f, 0.f), 
                       vec_new(1.f, 6.f, 0.f)};
    vec_t output[7] = {vec_new(1.f, 0.f, 0.f),
                       vec_new(1.f, 1.f, 0.f),
                       vec_new(1.f, 2.f, 0.f),
                       vec_new(1.f, 3.f, 0.f),
                       vec_new(1.f, 4.f, 0.f),
                       vec_new(1.f, 5.f, 0.f),
                       vec_new(1.f, 6.f, 0.f)};

    rasterize_and_assert(input, 
                         sizeof(input) / sizeof(vec_t),
                         output,
                         sizeof(output) / sizeof(vec_t),
                         0xAABBCCFF);
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

    vec_t input[2]  = {vec_new(1.f, 1.f, 0.f), 
                       vec_new(3.f, 5.f, 0.f)};
    vec_t points[5] = {vec_new(1.f, 1.f, 0.f),
                       vec_new(2.f, 2.f, 0.f),
                       vec_new(2.f, 3.f, 0.f),
                       vec_new(3.f, 4.f, 0.f),
                       vec_new(3.f, 5.f, 0.f)};

    rasterize_and_assert(input, 
                         sizeof(input) / sizeof(vec_t),
                         points,
                         sizeof(points) / sizeof(vec_t),
                         0xAABBCCFF);
}

static void test_rasterize_line_steep_pos_slope_out_of_bounds()
{
    // +--------------------+
    // |                    |
    // |                    |
    // |                    |
    // |                    |
    // |      xx            |
    // |    xx              |
    // |    xx              |
    // |  xx                |
    // |xx                  |
    // |xx                  |
    // +--------------------+

    vec_t input[2]  = {vec_new(-1.f, -1.f, 0.f), 
                       vec_new(3.f, 5.f, 0.f)};
    vec_t points[6] = {vec_new(0.f, 0.f, 0.f),
                       vec_new(0.f, 1.f, 0.f),
                       vec_new(1.f, 2.f, 0.f),
                       vec_new(2.f, 3.f, 0.f),
                       vec_new(2.f, 4.f, 0.f),
                       vec_new(3.f, 5.f, 0.f)};

    rasterize_and_assert(input, 
                         sizeof(input) / sizeof(vec_t),
                         points,
                         sizeof(points) / sizeof(vec_t),
                         0xAABBCCFF);
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

    vec_t input[2]  = {vec_new(1.f, 5.f, 0.f), 
                       vec_new(3.f, 1.f, 0.f)};
    vec_t points[5] = {vec_new(1.f, 5.f, 0.f),
                       vec_new(1.f, 4.f, 0.f),
                       vec_new(2.f, 3.f, 0.f),
                       vec_new(2.f, 2.f, 0.f),
                       vec_new(3.f, 1.f, 0.f)};

    rasterize_and_assert(input, 
                         sizeof(input) / sizeof(vec_t),
                         points,
                         sizeof(points) / sizeof(vec_t),
                         0xAABBCCFF);
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

    vec_t input[2]  = {vec_new(1.f, 1.f, 0.f), 
                       vec_new(5.f, 4.f, 0.f)};
    vec_t points[5] = {vec_new(1.f, 1.f, 0.f),
                       vec_new(2.f, 2.f, 0.f),
                       vec_new(3.f, 3.f, 0.f),
                       vec_new(4.f, 3.f, 0.f),
                       vec_new(5.f, 4.f, 0.f)};

    rasterize_and_assert(input, 
                         sizeof(input) / sizeof(vec_t),
                         points,
                         sizeof(points) / sizeof(vec_t),
                         0xAABBCCFF);
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

    vec_t input[2]  = {vec_new(1.f, 3.f, 0.f), 
                       vec_new(6.f, 1.f, 0.f)};
    vec_t points[6] = {vec_new(1.f, 3.f, 0.f),
                       vec_new(2.f, 3.f, 0.f),
                       vec_new(3.f, 2.f, 0.f),
                       vec_new(4.f, 2.f, 0.f),
                       vec_new(5.f, 1.f, 0.f),
                       vec_new(6.f, 1.f, 0.f)};

    rasterize_and_assert(input, 
                         sizeof(input) / sizeof(vec_t),
                         points,
                         sizeof(points) / sizeof(vec_t),
                         0xAABBCCFF);
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

    vec_t input[3]  = {vec_new(1.f, 1.f, 0.5f), 
                       vec_new(8.f, 1.f, 0.5f),
                       vec_new(3.f, 6.f, 0.5f)};
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

    rasterize_and_assert(input, 
                         sizeof(input) / sizeof(vec_t),
                         points,
                         sizeof(points) / sizeof(vec_t),
                         0xAABBCCFF);
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
    vec_t p1        = vec_new(1.f, 1.f, 0.5f);
    vec_t p2        = vec_new(8.f, 1.f, 0.5f);
    vec_t p3        = vec_new(3.f, 6.f, 0.5f);
    uint32_t color1 = 0xAABBCCFF;

    // assert initial values
    for (uint32_t i = 0; i < width; i++)
    {
        for (uint32_t j = 0; j < height; j++)
        {
            ASSERT_EQUAL(0, framebuffer_get(framebuffer, i, j));
            ASSERT_EQUAL(0.f, depthbuffer_get(depthbuffer, i, j));
        }
    }

    // rasterize first triangle
    rasterize_triangle(p1, p2, p3, color1, framebuffer, depthbuffer);

    p1 = vec_new(6.f, 6.f, 0.6f);
    p2 = vec_new(6.f, 1.f, 0.6f);
    p3 = vec_new(8.f, 1.f, 0.6f);
    uint32_t color2 = 0x11223344;

    // rasterize second triangle (closer to camera)
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
    vec_t points[19] = {vec_new(1.f, 1.f, 0.5f),
                        vec_new(2.f, 1.f, 0.5f),
                        vec_new(3.f, 1.f, 0.5f),
                        vec_new(4.f, 1.f, 0.5f),
                        vec_new(5.f, 1.f, 0.5f),
                        vec_new(2.f, 2.f, 0.5f),
                        vec_new(3.f, 2.f, 0.5f),
                        vec_new(4.f, 2.f, 0.5f),
                        vec_new(5.f, 2.f, 0.5f),
                        vec_new(2.f, 3.f, 0.5f),
                        vec_new(3.f, 3.f, 0.5f),
                        vec_new(4.f, 3.f, 0.5f),
                        vec_new(5.f, 3.f, 0.5f),
                        vec_new(3.f, 4.f, 0.5f),
                        vec_new(4.f, 4.f, 0.5f),
                        vec_new(5.f, 4.f, 0.5f),
                        vec_new(3.f, 5.f, 0.5f),
                        vec_new(4.f, 5.f, 0.5f),
                        vec_new(3.f, 6.f, 0.5f)};

    for (uint32_t i = 0; i < sizeof(points) / sizeof(vec_t); i++)
    {
        vec_t p = points[i];
        ASSERT_EQUAL(color1, framebuffer_get(framebuffer, (uint32_t)p.x, (uint32_t)p.y));
    }

    // assert second triangle
    vec_t points2[10] = {vec_new(6.f, 1.f, 0.6f),
                         vec_new(7.f, 1.f, 0.6f),
                         vec_new(8.f, 1.f, 0.6f),
                         vec_new(6.f, 2.f, 0.6f),
                         vec_new(7.f, 2.f, 0.6f),
                         vec_new(6.f, 3.f, 0.6f),
                         vec_new(7.f, 3.f, 0.6f),
                         vec_new(6.f, 4.f, 0.6f),
                         vec_new(6.f, 5.f, 0.6f),
                         vec_new(6.f, 6.f, 0.6f)};

    for (uint32_t i = 0; i < sizeof(points2) / sizeof(vec_t); i++)
    {
        vec_t p = points2[i];
        ASSERT_EQUAL(color2, framebuffer_get(framebuffer, (uint32_t)p.x, (uint32_t)p.y));
    }

    framebuffer_free(framebuffer);
    depthbuffer_free(depthbuffer);
}

static void test_rasterize_clipped_triangle()
{

    vec_t input[3]  = {vec_new(1.f, 1.f, 0.0f),
                       vec_new(8.f, 1.f, 0.5f),
                       vec_new(3.f, 6.f, 0.5f)};

    rasterize_and_assert(input,
                         sizeof(input) / sizeof(vec_t),
                         NULL,
                         0,
                         0xAABBCCFF);
}

static void test_rasterize_colinear_triangle()
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

    vec_t input[3]  = {vec_new(1.f, 1.f, 0.5f),
                       vec_new(6.f, 1.f, 0.5f),
                       vec_new(3.f, 1.f, 0.5f)};

    rasterize_and_assert(input,
                         sizeof(input) / sizeof(vec_t),
                         NULL,
                         0,
                         0xAABBCCFF);
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
    TEST_CASE(test_rasterize_line_horizontal_out_of_bounds);
    TEST_CASE(test_rasterize_line_vertical);
    TEST_CASE(test_rasterize_line_vertical_out_of_bounds);
    TEST_CASE(test_rasterize_line_steep_pos_slope);
    TEST_CASE(test_rasterize_line_steep_pos_slope_out_of_bounds);
    TEST_CASE(test_rasterize_line_steep_neg_slope);
    TEST_CASE(test_rasterize_line_pos_slope);
    TEST_CASE(test_rasterize_line_neg_slope);
    TEST_CASE(test_rasterize_triangle);
    TEST_CASE(test_rasterize_multiple_triangles);
    TEST_CASE(test_rasterize_clipped_triangle);
    TEST_CASE(test_rasterize_colinear_triangle);
}