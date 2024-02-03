#include "test_vector.h"
#include "test_utils.h"

#include <math.h>

static void test_vector_addition()
{
    vec_t v1        = vec_new(1.f, 2.f, 3.f);
    vec_t v2        = vec_new(3.f, 2.f, 1.f);
    vec_t actual    = vec_add(v1, v2);
    vec_t expected  = vec_new(4.f, 4.f, 4.f);

    ASSERT_VECTOR(expected, actual);
}

static void test_vector_subtraction()
{
    vec_t v1        = vec_new(1.f, 2.f, 3.f);
    vec_t v2        = vec_new(3.f, 2.f, 1.f);
    vec_t actual    = vec_sub(v1, v2);
    vec_t expected  = vec_new(-2.f, 0.f, 2.f);

    ASSERT_VECTOR(expected, actual);
}

static void test_vector_cross()
{
    vec_t v1        = vec_new(1.f, 2.f, 3.f);
    vec_t v2        = vec_new(2.f, 3.5f, 14.2f);
    vec_t actual    = vec_cross(v1, v2);
    vec_t expected  = vec_new(17.9f, -8.2f, -0.5f);

    ASSERT_VECTOR(expected, actual);
}

static void test_vector_scale()
{
    vec_t v1        = vec_new(1.f, 2.f, 3.f);
    vec_t actual    = vec_scale(v1, 5.f);
    vec_t expected  = vec_new(5.f, 10.f, 15.f);

    ASSERT_VECTOR(expected, actual);
}

static void test_vector_negate()
{
    vec_t v1        = vec_new(1.f, 2.f, 3.f);
    vec_t actual    = vec_negate(v1);
    vec_t expected  = vec_new(-1.f, -2.f, -3.f);

    ASSERT_VECTOR(expected, actual);
}

static void test_vector_normalize()
{
    vec_t v1        = vec_new(2.f, 3.f, 4.f);
    vec_t actual    = vec_normalize(v1);
    float scale     = sqrtf(2 * 2 + 3 * 3 + 4 * 4);
    vec_t expected  = vec_new(2.f/scale, 3.f/scale, 4.f/scale);

    ASSERT_VECTOR(expected, actual);
}

static void test_vector_hadamard()
{
    vec_t v1        = vec_new(1.f, 2.f, 3.f);
    vec_t v2        = vec_new(4.f, 5.f, 6.f);
    vec_t actual    = vec_hadamard(v1, v2);
    vec_t expected  = vec_new(4.f, 10.f, 18.f);

    ASSERT_VECTOR(expected, actual);
}

static void test_vector_clamp()
{
    vec_t v         = vec_new(-2.5f, 2.f, 0.5f);
    vec_t actual    = vec_clamp(v, 0.f, 1.f);
    vec_t expected  = vec_new(0.f, 1.f, 0.5f);

    ASSERT_VECTOR(expected, actual);
}

static void test_vector_dot()
{
    vec_t v1        = vec_new(1.f, 2.f, 3.f);
    vec_t v2        = vec_new(3.f, 2.f, 1.f);
    float actual    = vec_dot(v1, v2);
    float expected  = 10.f;

    ASSERT_EQUAL(expected, actual);
}

static void test_vector_magnitude()
{
    vec_t v1        = vec_new(2.f, 3.f, 4.f);
    float actual    = vec_magnitude(v1);
    float expected  = sqrtf(2 * 2 + 3 * 3 + 4 * 4);

    ASSERT_EQUAL(expected, actual);
}

static void test_vector_magnitude_sq()
{
    vec_t v1        = vec_new(2.f, 3.f, 4.f);
    float actual    = vec_magnitude_sq(v1);
    float expected  = 2 * 2 + 3 * 3 + 4 * 4;

    ASSERT_EQUAL(expected, actual);
}

static void test_vector_from_bgra()
{
    uint32_t color  = 0xFFAABBCC;
    vec_t actual    = vec_from_bgra(color);

    vec_t expected  = { .x = 255.f / 255.f,
                        .y = 170.f / 255.f,
                        .z = 187.f / 255.f,
                        .w = 204.f / 255.f };

    ASSERT_VECTOR(expected, actual);
}

static void test_vector_to_bgra()
{
    vec_t color         = { .x = 255.f / 255.f,
                            .y = 170.f / 255.f,
                            .z = 187.f / 255.f,
                            .w = 204.f / 255.f };
    uint32_t actual     = vec_to_bgra(color);
    uint32_t expected   = 0xFFAABBCC;

    ASSERT_EQUAL(expected, actual);
}

static void test_vector_from_scalar()
{
    float s = 1.f;
    vec_t actual = vec_from_scalar(s);
    vec_t expected = vec_new(1.f, 1.f, 1.f);

    ASSERT_VECTOR(expected, actual);
}

void test_vector()
{
    TEST_CASE(test_vector_addition);
    TEST_CASE(test_vector_subtraction);
    TEST_CASE(test_vector_cross);
    TEST_CASE(test_vector_scale);
    TEST_CASE(test_vector_negate);
    TEST_CASE(test_vector_normalize);
    TEST_CASE(test_vector_hadamard);
    TEST_CASE(test_vector_clamp);
    TEST_CASE(test_vector_dot);
    TEST_CASE(test_vector_magnitude);
    TEST_CASE(test_vector_magnitude_sq);
    TEST_CASE(test_vector_from_bgra);
    TEST_CASE(test_vector_to_bgra);
    TEST_CASE(test_vector_from_scalar);
}