#include "test_math_utils.h"

#include "test_utils.h"

static void test_f_abs()
{
    float actual;
    float expected;

    actual      = f_abs(4.f);
    expected    = 4.f;
    ASSERT_EQUAL(actual, expected);

    actual      = f_abs(-4.f);
    expected    = 4.f;
    ASSERT_EQUAL(actual, expected);

    actual      = f_abs(0.f);
    expected    = 0.f;
    ASSERT_EQUAL(actual, expected);
}


static void test_f_min()
{
    float actual;
    float expected;

    actual      = f_min(1.f, 2.f);
    expected    = 1.f;
    ASSERT_EQUAL(actual, expected);

    actual      = f_min(-1000.f, -2.f);
    expected    = -1000.f;
    ASSERT_EQUAL(actual, expected);

    actual      = f_min(1000.f, -2.f);
    expected    = -2.f;
    ASSERT_EQUAL(actual, expected);

    actual      = f_min(1000.f, 0.f);
    expected    = 0.f;
    ASSERT_EQUAL(actual, expected);
}


static void test_f_max()
{
    float actual;
    float expected;

    actual      = f_max(1.f, 2.f);
    expected    = 2.f;
    ASSERT_EQUAL(actual, expected);

    actual      = f_max(-1000.f, -2.f);
    expected    = -2.f;
    ASSERT_EQUAL(actual, expected);

    actual      = f_max(1000.f, -2.f);
    expected    = 1000.f;
    ASSERT_EQUAL(actual, expected);

    actual      = f_max(1000.f, 0.f);
    expected    = 1000.f;
    ASSERT_EQUAL(actual, expected);
}


static void test_i_min()
{
    int32_t actual;
    int32_t expected;

    actual      = i_min(1, 2);
    expected    = 1;
    ASSERT_EQUAL(actual, expected);

    actual      = i_min(-1000, -2);
    expected    = -1000;
    ASSERT_EQUAL(actual, expected);

    actual      = i_min(1000, -2);
    expected    = -2;
    ASSERT_EQUAL(actual, expected);

    actual      = i_min(1000, 0);
    expected    = 0;
    ASSERT_EQUAL(actual, expected);
}


static void test_i_max()
{
    int32_t actual;
    int32_t expected;

    actual      = i_max(1, 2);
    expected    = 2;
    ASSERT_EQUAL(actual, expected);

    actual      = i_max(-1000, -2);
    expected    = -2;
    ASSERT_EQUAL(actual, expected);

    actual      = i_max(1000, -2);
    expected    = 1000;
    ASSERT_EQUAL(actual, expected);

    actual      = i_max(1000, 0);
    expected    = 1000;
    ASSERT_EQUAL(actual, expected);
}


static void test_u_min()
{
    uint32_t actual;
    uint32_t expected;

    actual      = u_min(1, 2);
    expected    = 1;
    ASSERT_EQUAL(actual, expected);

    actual      = u_min(1000, 0);
    expected    = 0;
    ASSERT_EQUAL(actual, expected);
}


static void test_u_max()
{
    uint32_t actual;
    uint32_t expected;

    actual      = u_max(1, 2);
    expected    = 2;
    ASSERT_EQUAL(actual, expected);

    actual      = u_max(1000, 0);
    expected    = 1000;
    ASSERT_EQUAL(actual, expected);
}


static void test_f_clamp()
{
    float actual;
    float expected;

    actual      = f_clamp(1.f, 0.f, 0.5f);
    expected    = 0.5f;
    ASSERT_EQUAL(actual, expected);

    actual      = f_clamp(-1.f, 0.f, 0.5f);
    expected    = 0.f;
    ASSERT_EQUAL(actual, expected);

    actual      = f_clamp(3.f, 0.f, 1.f);
    expected    = 1.f;
    ASSERT_EQUAL(actual, expected);
}


void test_math_utils()
{
    TEST_CASE(test_f_abs);
    TEST_CASE(test_f_min);
    TEST_CASE(test_f_max);
    TEST_CASE(test_i_min);
    TEST_CASE(test_i_max);
    TEST_CASE(test_u_min);
    TEST_CASE(test_u_max);
    TEST_CASE(test_f_clamp);
}