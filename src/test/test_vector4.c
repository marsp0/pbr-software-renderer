#include "test_vector4.h"
#include "test_utils.h"

#include <math.h>

static void test_vector_addition()
{
    vec4_t v1 = vec4_new(1.f, 2.f, 3.f);
    vec4_t v2 = vec4_new(3.f, 2.f, 1.f);
    vec4_t actual = vec4_add(v1, v2);
    vec4_t expected = vec4_new(4.f, 4.f, 4.f);
    ASSERT_VECTOR4(expected, actual);
}

static void test_vector_subtraction()
{
    vec4_t v1 = vec4_new(1.f, 2.f, 3.f);
    vec4_t v2 = vec4_new(3.f, 2.f, 1.f);
    vec4_t actual = vec4_sub(v1, v2);
    vec4_t expected = vec4_new(-2.f, 0.f, 2.f);
    ASSERT_VECTOR4(expected, actual);
}

static void test_vector_cross()
{
    vec4_t v1 = vec4_new(1.f, 2.f, 3.f);
    vec4_t v2 = vec4_new(2.f, 3.5f, 14.2f);
    vec4_t actual = vec4_cross(v1, v2);
    vec4_t expected = vec4_new(17.9f, -8.2f, -0.5f);
    ASSERT_VECTOR4(expected, actual);
}

static void test_vector_scale()
{
    vec4_t v1 = vec4_new(1.f, 2.f, 3.f);
    vec4_t actual = vec4_scale(v1, 5.f);
    vec4_t expected = vec4_new(5.f, 10.f, 15.f);
    ASSERT_VECTOR4(expected, actual);
}

static void test_vector_negate()
{
    vec4_t v1 = vec4_new(1.f, 2.f, 3.f);
    vec4_t actual = vec4_negate(v1);
    vec4_t expected = vec4_new(-1.f, -2.f, -3.f);
    ASSERT_VECTOR4(expected, actual);
}

static void test_vector_normalize()
{
    vec4_t v1 = vec4_new(2.f, 3.f, 4.f);
    vec4_t actual = vec4_normalize(v1);
    float scale = sqrtf(2 * 2 + 3 * 3 + 4 * 4);
    vec4_t expected = vec4_new(2.f/scale, 3.f/scale, 4.f/scale);
    ASSERT_VECTOR4(expected, actual);
}

static void test_vector_dot()
{
    vec4_t v1 = vec4_new(1.f, 2.f, 3.f);
    vec4_t v2 = vec4_new(3.f, 2.f, 1.f);
    float actual = vec4_dot(v1, v2);
    float expected = 10.f;
    ASSERT_EQUAL(expected, actual);
}

static void test_vector_magnitude()
{
    vec4_t v1 = vec4_new(2.f, 3.f, 4.f);
    float actual = vec4_magnitude(v1);
    float expected = sqrtf(2 * 2 + 3 * 3 + 4 * 4);
    ASSERT_EQUAL(expected, actual);
}

static void test_vector_magnitude_sq()
{
    vec4_t v1 = vec4_new(2.f, 3.f, 4.f);
    float actual = vec4_magnitude_sq(v1);
    float expected = 2 * 2 + 3 * 3 + 4 * 4;
    ASSERT_EQUAL(expected, actual);
}

static void test_vector_hadamard_product()
{
    vec4_t v1 = vec4_new(2.f, 3.f, 4.f);
    vec4_t v2 = vec4_new(3.f, 4.f, 5.f);
    vec4_t actual = vec4_hadamard(v1, v2);
    vec4_t expected = vec4_new(6.f, 12.f, 20.f);

    ASSERT_VECTOR4(expected, actual);
}

static void test_vector_scale_with_w()
{
    vec4_t p        = vec4_new(1.f, 2.f, 3.f);
    vec4_t actual   = vec4_scale_with_w(p, 2.f);
    vec4_t expected = vec4_new(2.f, 4.f, 6.f);
    expected.w      = 2.f;

    ASSERT_VECTOR4(actual, expected);
}

void test_vector4()
{
    TEST_CASE(test_vector_addition);
    TEST_CASE(test_vector_subtraction);
    TEST_CASE(test_vector_cross);
    TEST_CASE(test_vector_scale);
    TEST_CASE(test_vector_negate);
    TEST_CASE(test_vector_normalize);
    TEST_CASE(test_vector_dot);
    TEST_CASE(test_vector_magnitude);
    TEST_CASE(test_vector_magnitude_sq);
    TEST_CASE(test_vector_hadamard_product);
    TEST_CASE(test_vector_scale_with_w);
}