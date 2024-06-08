#include "test_math_utils.h"

#include "test_utils.h"

#include "../math.h"

static void test_max_functions()
{
    int32_t a = 2;
    int32_t b = -3;
    
    uint32_t c = 3;
    uint32_t d = 4;
    
    ASSERT_EQUAL(i_max(a, b), a);
    ASSERT_EQUAL(u_max(c, d), d);
}

static void test_min_functions()
{
    int32_t a = 2;
    int32_t b = -3;
    
    uint32_t c = 3;
    uint32_t d = 4;
    
    ASSERT_EQUAL(i_min(a, b), b);
    ASSERT_EQUAL(u_min(c, d), c);
}

void test_math_utils()
{
    TEST_CASE(test_max_functions);
    TEST_CASE(test_min_functions);
}