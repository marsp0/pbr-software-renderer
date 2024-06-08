#include "test_vec2.h"
#include "test_utils.h"

#include <math.h>

static void test_vector_addition()
{
    vec2_t v1       = vec2_new(1.f, 2.f);
    vec2_t v2       = vec2_new(3.f, 2.f);
    vec2_t actual   = vec2_add(v1, v2);
    vec2_t expected = vec2_new(4.f, 4.f);

    ASSERT_VECTOR2(expected, actual);
}

static void test_vector_subtraction()
{
    vec2_t v1       = vec2_new(1.f, 2.f);
    vec2_t v2       = vec2_new(3.f, 2.f);
    vec2_t actual   = vec2_sub(v1, v2);
    vec2_t expected = vec2_new(-2.f, 0.f);

    ASSERT_VECTOR2(expected, actual);
}

static void test_vector_scale()
{
    vec2_t v1       = vec2_new(1.f, 2.f);
    vec2_t actual   = vec2_scale(v1, 5.f);
    vec2_t expected = vec2_new(5.f, 10.f);

    ASSERT_VECTOR2(expected, actual);
}

static void test_vector_negate()
{
    vec2_t v1       = vec2_new(1.f, 2.f);
    vec2_t actual   = vec2_negate(v1);
    vec2_t expected = vec2_new(-1.f, -2.f);

    ASSERT_VECTOR2(expected, actual);
}

static void test_vector_normalize()
{
    vec2_t v1       = vec2_new(2.f, 3.f);
    vec2_t actual   = vec2_normalize(v1);
    float scale     = sqrtf(2 * 2 + 3 * 3);
    vec2_t expected = vec2_new(2.f / scale, 3.f / scale);

    ASSERT_VECTOR2(expected, actual);
}

static void test_vector_hadamard()
{
    vec2_t v1       = vec2_new(1.f, 2.f);
    vec2_t v2       = vec2_new(4.f, 5.f);
    vec2_t actual   = vec2_hadamard(v1, v2);
    vec2_t expected = vec2_new(4.f, 10.f);

    ASSERT_VECTOR2(expected, actual);
}

static void test_vector_clamp()
{
    vec2_t v        = vec2_new(-2.5f, 2.f);
    vec2_t actual   = vec2_clamp(v, 0.f, 1.f);
    vec2_t expected = vec2_new(0.f, 1.f);

    ASSERT_VECTOR2(expected, actual);
}

static void test_vector_dot()
{
    vec2_t v1       = vec2_new(1.f, 2.f);
    vec2_t v2       = vec2_new(3.f, 2.f);
    float actual    = vec2_dot(v1, v2);
    float expected  = 7.f;

    ASSERT_EQUAL(expected, actual);
}

static void test_vector_magnitude()
{
    vec2_t v1       = vec2_new(2.f, 3.f);
    float actual    = vec2_magnitude(v1);
    float expected  = sqrtf(2 * 2 + 3 * 3);

    ASSERT_EQUAL(expected, actual);
}

static void test_vector_magnitude_sq()
{
    vec2_t v1       = vec2_new(2.f, 3.f);
    float actual    = vec2_magnitude_sq(v1);
    float expected  = 2 * 2 + 3 * 3;

    ASSERT_EQUAL(expected, actual);
}

static void test_vector_from_scalar()
{
    float s             = 1.f;
    vec2_t actual       = vec2_from_scalar(s);
    vec2_t expected     = vec2_new(1.f, 1.f);

    ASSERT_VECTOR2(expected, actual);
}

void test_vec2()
{
    TEST_CASE(test_vector_addition);
    TEST_CASE(test_vector_subtraction);
    TEST_CASE(test_vector_scale);
    TEST_CASE(test_vector_negate);
    TEST_CASE(test_vector_normalize);
    TEST_CASE(test_vector_hadamard);
    TEST_CASE(test_vector_clamp);
    TEST_CASE(test_vector_dot);
    TEST_CASE(test_vector_magnitude);
    TEST_CASE(test_vector_magnitude_sq);
    TEST_CASE(test_vector_from_scalar);
}