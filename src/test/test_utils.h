#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "../math.h"

void        increment_test_assert_counter();
void        reset_test_assert_counter();
uint32_t    get_test_assert_counter();
void        INIT_TESTS();
void        TESTS_SUMMARY();

// NOTE: how does this work?
#define GET_COMPARISON(a, b) _Generic(  a, \
                                        float: fabs((float)a - (float)b) > 0.0005f, \
                                        default: a != b)

#define GET_FORMAT(a) _Generic( a, \
                                int:        "%010d  != %010d  (%20s != %-20s)\n", \
                                int64_t:    "%010ld != %010ld (%20s != %-20s)\n", \
                                uint32_t:   "%010u  != %010u  (%20s != %-20s)\n", \
                                uint64_t:   "%010lu != %010lu (%20s != %-20s)\n", \
                                char:       "%c != %c    (%20s != %-20s)\n", \
                                float:      "%010f  != %010f  (%20s != %-20s)\n", \
                                bool:       "%010d  != %010d  (%20s != %-20s)\n")

#define ASSERT_EQUAL(a, b)  do \
                            { \
                                bool fail = GET_COMPARISON(a, b); \
                                if (fail) \
                                { \
                                    increment_test_assert_counter(); \
                                } \
                                if (fail && get_test_assert_counter() <= 10) \
                                { \
                                    printf("\t\tAssert fail: "); \
                                    printf(GET_FORMAT(a), a, b, #a, #b); \
                                } \
                            } while(0);

#define ASSERT_TRUE(a)  do \
                        { \
                            ASSERT_EQUAL((a), true); \
                        } while(0);

#define ASSERT_FALSE(a) do \
                        { \
                            ASSERT_EQUAL((a), false); \
                        } while(0);

#define ASSERT_POINTER(a, b) ASSERT_EQUAL((int64_t)a, (int64_t)b)

#define ASSERT_STRING(a, b, size)   do \
                                    { \
                                        for (uint32_t i = 0; i < size; i++) \
                                        { \
                                            ASSERT_EQUAL(a[i], b[i]); \
                                        } \
                                    } while(0);

#define ASSERT_VECTOR(a, b) do \
                            { \
                                ASSERT_EQUAL(a.x, b.x); \
                                ASSERT_EQUAL(a.y, b.y); \
                                ASSERT_EQUAL(a.z, b.z); \
                            } while(0);

#define ASSERT_MATRIX(a, b) do \
                            { \
                                for (uint32_t i = 0; i < 4; i++) \
                                { \
                                    for (uint32_t j = 0; j < 4; j++) \
                                    { \
                                        ASSERT_EQUAL(a.data[i][j], b.data[i][j]); \
                                    } \
                                } \
                            } while(0);

#define TEST_CASE(test) do \
                        { \
                            printf("\t%s\n", #test); \
                            reset_test_assert_counter(); \
                            test(); \
                            uint32_t count = get_test_assert_counter(); \
                            if (count > 10) \
                            { \
                                printf("\t\t ... %u more assert fails\n", count - 10); \
                            } \
                        } while(0);

#define TEST_GROUP(group)   do \
                            { \
                                printf("%s\n", #group); \
                                group(); \
                            } while(0);
