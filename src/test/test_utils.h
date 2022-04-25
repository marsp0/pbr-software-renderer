#pragma once

#include "../vector.h"


void assert_vec(vec expected, vec actual, const char* message, const char* file_name, int line_number);
void assert_float(float expected, float actual, const char* message, const char* file_name, int line_number);

#define ASSERT_VEC(expected, actual) assert_vec(expected, actual, "", __FILE__, __LINE__);
#define ASSERT_VEC_MSG(expected, actual, message) assert_vec(expected, actual, message, __FILE__, __LINE__);
#define ASSERT_FLOAT(expected, actual) assert_float(expected, actual, "", __FILE__, __LINE__);
#define ASSERT_FLOAT_MSG(expected, actual, message) assert_float(expected, actual, message, __FILE__, __LINE__);
