#include "test_utils.h"

#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

static float epsilon = 0.0005f;

void assert_vec(vec_t expected, vec_t actual, const char* file_name, int32_t line_number)
{
    bool result = true;
    float x_diff = (float)fabs(expected.x - actual.x);
    float y_diff = (float)fabs(expected.y - actual.y);
    float z_diff = (float)fabs(expected.z - actual.z);
    if (x_diff > epsilon || y_diff > epsilon || z_diff > epsilon)
        result = false;
    
    if (!result)
    {
        printf("File:\t\t %s\n", file_name);
        printf("Line:\t\t %i\n", line_number);
        printf("Actual:\t\t %f, %f, %f\n", actual.x, actual.y, actual.z);
        printf("Expected:\t %f, %f, %f\n", expected.x, expected.y, expected.z);
    }

    assert(result);
}

void assert_float(float expected, float actual, const char* file_name, int32_t line_number)
{
    float diff = (float)fabs(expected - actual);
    float result = true;
    if (diff > epsilon)
    {
        result = false;
        printf("File:\t\t %s\n", file_name);
        printf("Line:\t\t %i\n", line_number);
        printf("Actual:\t\t %f\n", actual);
        printf("Expected:\t %f\n", expected);
    }
    assert(result);
}

void assert_int(int32_t expected, int32_t actual, const char* file_name, int32_t line_number)
{
    if (expected != actual)
    {
        printf("File:\t\t %s\n", file_name);
        printf("Line:\t\t %i\n", line_number);
        printf("Actual:\t\t %d\n", actual);
        printf("Expected:\t %d\n", expected);
        assert(false);
    }
}

void assert_mat(mat_t expected, mat_t actual, const char* file_name, int32_t line_number)
{
    bool result = true;
    for (int32_t i = 0; i < 4; i++)
    {
        for (int32_t j = 0; j < 4; j++)
        {
            if (fabs(expected.data[i][j] - actual.data[i][j]) > epsilon)
                result = false;
        }
    }
    
    if (!result)
    {
        printf("File:\t\t %s\n", file_name);
        printf("Line:\t\t %i\n", line_number);
        printf("Actual/Expected:\n");
        for (int32_t i = 0; i < 4; i++)
        {
            for (int32_t j = 0; j < 4; j++)
            {
                printf("%9.5f|%9.5f\t", actual.data[i][j], expected.data[i][j]);
            }
            printf("\n");
        }
    }

    assert(result);
}

void assert_col(color expected, color actual, const char* file_name, int32_t line_number)
{
    bool result = true;
    float r_diff = (float)fabs(expected.r - actual.r);
    float g_diff = (float)fabs(expected.g - actual.g);
    float b_diff = (float)fabs(expected.b - actual.b);
    if (r_diff > epsilon || g_diff > epsilon || b_diff > epsilon)
        result = false;
    
    if (!result)
    {
        printf("File:\t\t %s\n", file_name);
        printf("Line:\t\t %i\n", line_number);
        printf("Actual:\t\t %f, %f, %f\n", actual.r, actual.g, actual.b);
        printf("Expected:\t %f, %f, %f\n", expected.r, expected.g, expected.b);
    }

    assert(result);
}

void assert_string(const char* expected, const char* actual, uint32_t size, const char* file_name, int32_t line_number)
{
    bool result = true;
    for (uint32_t i = 0; i < size; i++)
    {
        if (expected[i] != actual[i])
        {
            result = false;
        }
    }

    if (!result)
    {
        printf("File:\t\t %s\n", file_name);
        printf("Line:\t\t %i\n", line_number);
        printf("Actual:\t\t %.*s\n", size, actual);
        printf("Expected:\t %.*s\n", size, expected);
    }

    assert(result);
}

void assert_pointer_exists(const void* pointer, const char* file_name, int32_t line_number)
{
    if (!pointer)
    {
        printf("File:\t\t %s\n", file_name);
        printf("Line:\t\t %i\n", line_number);
        printf("Pointer is NULL\n");
        assert(false);
    }
}

void assert_pointer(const void* expected, const void* actual, const char* file_name, int32_t line_number)
{
    if (expected != actual)
    {
        printf("File:\t\t %s\n", file_name);
        printf("Line:\t\t %i\n", line_number);
        printf("Actual:\t\t %p\n", actual);
        printf("Expected:\t %p\n", expected);
        assert(false);
    }
}

void assert_uint(uint32_t expected, uint32_t actual, bool is_hex, const char* file_name, int32_t line_number)
{
    if (expected != actual)
    {
        printf("File:\t\t %s\n", file_name);
        printf("Line:\t\t %i\n", line_number);
        if (is_hex)
        {
            printf("Actual:\t\t %x\n", actual);
            printf("Expected:\t %x\n", expected);
        }
        else
        {
            printf("Actual:\t\t %u\n", actual);
            printf("Expected:\t %u\n", expected);
        }

        assert(false);
    }
}

void assert_bool(bool expected, bool actual, const char* file_name, int32_t line_number)
{
    if (expected != actual)
    {
        printf("File:\t\t %s\n", file_name);
        printf("Line:\t\t %i\n", line_number);
        printf("Actual:\t\t %d\n", actual);
        printf("Expected:\t %d\n", expected);
        assert(false);
    }
}