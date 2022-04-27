#include "test_utils.h"

#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

static float epsilon = 0.0005f;

void assert_vec(vec expected, vec actual, const char* message, const char* file_name, int line_number)
{
    bool result = true;
    float x_diff = fabs(expected.x - actual.x);
    float y_diff = fabs(expected.y - actual.y);
    float z_diff = fabs(expected.z - actual.z);
    if (x_diff > epsilon || y_diff > epsilon || z_diff > epsilon)
        result = false;
    
    if (!result)
    {
        if (strcmp(message, "") != 0)
        {
            printf(message);
        }
        printf("File:\t\t %s\n", file_name);
        printf("Line:\t\t %i\n", line_number);
        printf("Actual:\t\t %f, %f, %f\n", actual.x, actual.y, actual.z);
        printf("Expected:\t %f, %f, %f\n", expected.x, expected.y, expected.z);
    }

    assert(result);
}

void assert_float(float expected, float actual, const char* message, const char* file_name, int line_number)
{
    float diff = fabs(expected - actual);
    float result = true;
    if (diff > epsilon)
    {
        result = false;
        if (strcmp(message, "") != 0)
        {
            printf(message);
        }
        printf("File:\t\t %s\n", file_name);
        printf("Line:\t\t %i\n", line_number);
        printf("Actual:\t\t %f\n", actual);
        printf("Expected:\t %f\n", expected);
    }
    assert(result);
}

void assert_mat(mat expected, mat actual, const char* message, const char* file_name, int line_number)
{
    bool result = true;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (fabs(expected.data[i][j] - actual.data[i][j]) > epsilon)
                result = false;
        }
    }
    
    if (!result)
    {
        if (strcmp(message, "") != 0)
        {
            printf(message);
        }
        printf("File:\t\t %s\n", file_name);
        printf("Line:\t\t %i\n", line_number);
        printf("Actual/Expected:\n");
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                printf("%9.5f|%9.5f\t", actual.data[i][j], expected.data[i][j]);
            }
            printf("\n");
        }
    }

    assert(result);
}