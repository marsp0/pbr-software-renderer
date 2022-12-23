#pragma once

#include "../math.h"
#include "../color.h"


void assert_vec(vec_t expected, vec_t actual, const char* message, const char* file_name, int line_number);
void assert_float(float expected, float actual, const char* message, const char* file_name, int line_number);
void assert_int(int expected, int actual, const char* message, const char* file_name, int line_number);
void assert_mat(mat_t expected, mat_t actual, const char* message, const char* file_name, int line_number);
void assert_col(color expected, color actual, const char* message, const char* file_name, int line_number);
void assert_string(const char* expected, const char* actual, int size, const char* file_name, int line_number);
void assert_pointer(void* expected, void* actual, const char* file_name, int line_number);

#define ASSERT_VEC(expected, actual) assert_vec(expected, actual, "", __FILE__, __LINE__);
#define ASSERT_VEC_MSG(expected, actual, message) assert_vec(expected, actual, message, __FILE__, __LINE__);
#define ASSERT_FLOAT(expected, actual) assert_float(expected, actual, "", __FILE__, __LINE__);
#define ASSERT_FLOAT_MSG(expected, actual, message) assert_float(expected, actual, message, __FILE__, __LINE__);
#define ASSERT_INT(expected, actual) assert_int(expected, actual, "", __FILE__, __LINE__);
#define ASSERT_INT_MSG(expected, actual, message) assert_int(expected, actual, message, __FILE__, __LINE__);
#define ASSERT_MAT(expected, actual) assert_mat(expected, actual, "", __FILE__, __LINE__);
#define ASSERT_MAT_MSG(expected, actual, message) assert_mat(expected, actual, message, __FILE__, __LINE__);
#define ASSERT_COL(expected, actual) assert_col(expected, actual, "", __FILE__, __LINE__);
#define ASSERT_COL_MSG(expected, actual, message) assert_col(expected, actual, message, __FILE__, __LINE__);
#define ASSERT_STRING(expected, actual, size) assert_string(expected, actual, size, __FILE__, __LINE__);
#define ASSERT_POINTER(expected, actual) assert_pointer(expected, actual, __FILE__, __LINE__);