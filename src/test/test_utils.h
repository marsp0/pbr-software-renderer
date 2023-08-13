#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "../math.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


void        increment_test_assert_counter();
void        reset_test_assert_counter();
uint32_t    get_test_assert_counter();
void        INIT_TESTS();
void        TESTS_SUMMARY();

#define GET_COMPARISON(a, b) _Generic(  a, \
                                        float: fabs((float)a - (float)b) > 0.0005f, \
                                        default: a != b)

#define GET_FORMAT(a) _Generic( a, \
                                int:         "%d != %d (%s != %s)\n", \
                                int64_t:    "%ld != %ld (%s != %s)\n", \
                                uint32_t:    "%u != %u (%s != %s)\n", \
                                uint64_t:   "%lu != %lu (%s != %s)\n", \
                                char:        "%c != %c (%s != %s)\n", \
                                float:       "%f != %f (%s != %s)\n", \
                                bool:        "%d != %d (%s != %s)\n")


#define ASSERT_EQUAL(a, b)          do \
                                    { \
                                        bool fail = GET_COMPARISON(a, b); \
                                        if (fail) \
                                        { \
                                            increment_test_assert_counter(); \
                                        } \
                                        if (fail && get_test_assert_counter() <= 10) \
                                        { \
                                            printf("    [" ANSI_COLOR_RED "FAIL" ANSI_COLOR_RESET "]: "); \
                                            printf(GET_FORMAT(a), a, b, #a, #b); \
                                        } \
                                    } while(0);


#define ASSERT_TRUE(a)              do \
                                    { \
                                        ASSERT_EQUAL((a), true); \
                                    } while(0);


#define ASSERT_FALSE(a)             do \
                                    { \
                                        ASSERT_EQUAL((a), false); \
                                    } while(0);


#define ASSERT_POINTER(a, b)        do \
                                    { \
                                        ASSERT_EQUAL((int64_t)a, (int64_t)b); \
                                    } while(0);


#define ASSERT_STRING(a, b, size)   do \
                                    { \
                                        for (uint32_t i = 0; i < size; i++) \
                                        { \
                                            ASSERT_EQUAL(a[i], b[i]); \
                                        } \
                                    } while(0);


#define ASSERT_VECTOR(a, b)         do \
                                    { \
                                        ASSERT_EQUAL(a.x, b.x); \
                                        ASSERT_EQUAL(a.y, b.y); \
                                        ASSERT_EQUAL(a.z, b.z); \
                                    } while(0);


#define ASSERT_MATRIX(a, b)         do \
                                    { \
                                        for (uint32_t i = 0; i < 4; i++) \
                                        { \
                                            for (uint32_t j = 0; j < 4; j++) \
                                            { \
                                                ASSERT_EQUAL(a.data[i][j], b.data[i][j]); \
                                            } \
                                        } \
                                    } while(0);


#define TEST_CASE(test)             do \
                                    { \
                                        printf("  %s\n", #test); \
                                        reset_test_assert_counter(); \
                                        test(); \
                                        uint32_t count = get_test_assert_counter(); \
                                        if (count > 10) \
                                        { \
                                            printf("    ... %u more assert fails\n", count - 10); \
                                        } \
                                    } while(0);


#define TEST_GROUP(group)           do \
                                    { \
                                        printf("%s\n", #group); \
                                        group(); \
                                    } while(0);
