#pragma once

#include "../math.h"
#include "../color.h"


void assert_vec(vec expected, vec actual, const char* message, const char* file_name, int line_number);
void assert_float(float expected, float actual, const char* message, const char* file_name, int line_number);
void assert_mat(mat expected, mat actual, const char* message, const char* file_name, int line_number);
void assert_col(color expected, color actual, const char* message, const char* file_name, int line_number);

#define ASSERT_VEC(expected, actual) assert_vec(expected, actual, "", __FILE__, __LINE__);
#define ASSERT_VEC_MSG(expected, actual, message) assert_vec(expected, actual, message, __FILE__, __LINE__);
#define ASSERT_FLOAT(expected, actual) assert_float(expected, actual, "", __FILE__, __LINE__);
#define ASSERT_FLOAT_MSG(expected, actual, message) assert_float(expected, actual, message, __FILE__, __LINE__);
#define ASSERT_MAT(expected, actual) assert_mat(expected, actual, "", __FILE__, __LINE__);
#define ASSERT_MAT_MSG(expected, actual, message) assert_mat(expected, actual, message, __FILE__, __LINE__);
#define ASSERT_COL(expected, actual) assert_col(expected, actual, "", __FILE__, __LINE__);
#define ASSERT_COL_MSG(expected, actual, message) assert_col(expected, actual, message, __FILE__, __LINE__);
