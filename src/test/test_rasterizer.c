#include "test_rasterizer.h"

#include "test_utils.h"

#include "../rasterizer.h"
#include "../rasterizer_constants.h"

static void test_draw_line_horizontal()
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
            uint32_t index = (j * width + i) * RGB_CHANNELS;
            ASSERT_UINT(buffer->data[index + 0], 0);
            ASSERT_UINT(buffer->data[index + 1], 0);
            ASSERT_UINT(buffer->data[index + 2], 0);
            ASSERT_UINT(buffer->data[index + 3], 0);
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

    ASSERT_UINT(expected_count, actual_count);

    // assert individual pixels
    for (uint32_t i = 324; i < 348; i += RGB_CHANNELS)
    {
        ASSERT_UINT(0xAA, buffer->data[i + 0]);
        ASSERT_UINT(0xBB, buffer->data[i + 1]);
        ASSERT_UINT(0xCC, buffer->data[i + 2]);
        ASSERT_UINT(0xFF, buffer->data[i + 3]);
    }
}

static void test_draw_line_vertical()
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
            uint32_t index = (j * width + i) * RGB_CHANNELS;
            ASSERT_UINT(buffer->data[index + 0], 0);
            ASSERT_UINT(buffer->data[index + 1], 0);
            ASSERT_UINT(buffer->data[index + 2], 0);
            ASSERT_UINT(buffer->data[index + 3], 0);
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

    ASSERT_UINT(expected_count, actual_count);

    // assert individual pixels
    for (uint32_t i = 124; i <= 324; i += 40)
    {
        ASSERT_UINT(0xAA, buffer->data[i + 0]);
        ASSERT_UINT(0xBB, buffer->data[i + 1]);
        ASSERT_UINT(0xCC, buffer->data[i + 2]);
        ASSERT_UINT(0xFF, buffer->data[i + 3]);
    }
}

static void test_draw_line_steep_pos_slope()
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
            uint32_t index = (j * width + i) * RGB_CHANNELS;
            ASSERT_UINT(buffer->data[index + 0], 0);
            ASSERT_UINT(buffer->data[index + 1], 0);
            ASSERT_UINT(buffer->data[index + 2], 0);
            ASSERT_UINT(buffer->data[index + 3], 0);
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

    ASSERT_UINT(expected_count, actual_count);

    // assert individual pixels
    uint32_t index = 324; // index of 1,1

    // 1, 1
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);

    // 2, 2
    index -= 36;
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);

    // 2, 3
    index -= 40;
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);

    // 3, 4
    index -= 36;
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);

    // 3, 5
    index -= 40;
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);
}

static void test_draw_line_steep_neg_slope()
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
            uint32_t index = (j * width + i) * RGB_CHANNELS;
            ASSERT_UINT(buffer->data[index + 0], 0);
            ASSERT_UINT(buffer->data[index + 1], 0);
            ASSERT_UINT(buffer->data[index + 2], 0);
            ASSERT_UINT(buffer->data[index + 3], 0);
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

    ASSERT_UINT(expected_count, actual_count);

    // assert individual pixels
    uint32_t index = 164; // index of 1,5

    // 1, 5
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);

    // 1, 4
    index += 40;
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);

    // 2, 3
    index += 44;
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);

    // 2, 2
    index += 40;
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);

    // 3, 1
    index += 44;
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);
}

static void test_draw_line_pos_slope()
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
            uint32_t index = (j * width + i) * RGB_CHANNELS;
            ASSERT_UINT(buffer->data[index + 0], 0);
            ASSERT_UINT(buffer->data[index + 1], 0);
            ASSERT_UINT(buffer->data[index + 2], 0);
            ASSERT_UINT(buffer->data[index + 3], 0);
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

    ASSERT_UINT(expected_count, actual_count);

    // assert individual pixels
    uint32_t index = 324; // index of 1,1

    // 1, 1
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);

    // 2, 2
    index -= 36;
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);

    // 3, 3
    index -= 36;
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);

    // 4, 3
    index += 4;
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);

    // 5, 4
    index -= 36;
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);
}

static void test_draw_line_neg_slope()
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
            uint32_t index = (j * width + i) * RGB_CHANNELS;
            ASSERT_UINT(buffer->data[index + 0], 0);
            ASSERT_UINT(buffer->data[index + 1], 0);
            ASSERT_UINT(buffer->data[index + 2], 0);
            ASSERT_UINT(buffer->data[index + 3], 0);
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

    ASSERT_UINT(expected_count, actual_count);

    // assert individual pixels
    uint32_t index = 244; // index of 1,3

    // 1, 3
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);

    // 2, 3
    index += 4;
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);

    // 3, 2
    index += 44;
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);

    // 4, 2
    index += 4;
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);

    // 5, 1
    index += 44;
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);

    // 6, 1
    index += 4;
    ASSERT_UINT(0xAA, buffer->data[index + 0]);
    ASSERT_UINT(0xBB, buffer->data[index + 1]);
    ASSERT_UINT(0xCC, buffer->data[index + 2]);
    ASSERT_UINT(0xFF, buffer->data[index + 3]);
}

void test_rasterizer()
{
    test_draw_line_horizontal();
    test_draw_line_vertical();
    test_draw_line_steep_pos_slope();
    test_draw_line_steep_neg_slope();
    test_draw_line_pos_slope();
    test_draw_line_neg_slope();
}