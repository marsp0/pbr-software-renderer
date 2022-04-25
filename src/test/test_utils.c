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