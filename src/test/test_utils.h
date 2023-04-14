#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../math.h"
#include "../color.h"

void assert_vec(vec_t expected, vec_t actual, const char* file_name, int32_t line_number);
void assert_float(float expected, float actual, const char* file_name, int32_t line_number);
void assert_int(int32_t expected, int32_t actual, const char* file_name, int32_t line_number);
void assert_mat(mat_t expected, mat_t actual, const char* file_name, int32_t line_number);
void assert_col(color expected, color actual, const char* file_name, int32_t line_number);
void assert_string(const char* expected, const char* actual, uint32_t size, const char* file_name, int32_t line_number);
void assert_pointer(const void* expected,const void* actual, const char* file_name, int32_t line_number);
void assert_uint(uint32_t expected, uint32_t actual, bool is_hex, const char* file_name, int32_t line_number);
void assert_bool(bool expected, bool actual, const char* file_name, int32_t line_number);

#define ASSERT_VEC(expected, actual) assert_vec(expected, actual, "", __FILE__, __LINE__);
#define ASSERT_FLOAT(expected, actual) assert_float(expected, actual, "", __FILE__, __LINE__);
#define ASSERT_INT(expected, actual) assert_int(expected, actual, "", __FILE__, __LINE__);
#define ASSERT_MAT(expected, actual) assert_mat(expected, actual, "", __FILE__, __LINE__);
#define ASSERT_COL(expected, actual) assert_col(expected, actual, "", __FILE__, __LINE__);
#define ASSERT_STRING(expected, actual, size) assert_string(expected, actual, size, __FILE__, __LINE__);
#define ASSERT_POINTER(expected, actual) assert_pointer(expected, actual, __FILE__, __LINE__);
#define ASSERT_UINT(expected, actual) assert_uint(expected, actual, false, __FILE__, __LINE__);
#define ASSERT_HEX(expected, actual) assert_uint(expected, actual, true, __FILE__, __LINE__);
#define ASSERT_BOOL(expected, actual) assert_bool(expected, actual, __FILE__, __LINE__);