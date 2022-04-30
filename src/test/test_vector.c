#include "test_vector.h"

#include "math.h"

#include "../vector.h"
#include "test_utils.h"

static void test_vector_addition()
{
    vec v1 = vec_new(1.f, 2.f, 3.f);
    vec v2 = vec_new(3.f, 2.f, 1.f);
    vec actual = vec_add(v1, v2);
    vec expected = vec_new(4.f, 4.f, 4.f);
    ASSERT_VEC(expected, actual);
}

static void test_vector_subtraction()
{
    vec v1 = vec_new(1.f, 2.f, 3.f);
    vec v2 = vec_new(3.f, 2.f, 1.f);
    vec actual = vec_sub(v1, v2);
    vec expected = vec_new(-2.f, 0.f, 2.f);
    ASSERT_VEC(expected, actual);
}

static void test_vector_cross()
{
    vec v1 = vec_new(1.f, 2.f, 3.f);
    vec v2 = vec_new(2.f, 3.5f, 14.2f);
    vec actual = vec_cross(v1, v2);
    vec expected = vec_new(17.9f, -8.2f, -0.5f);
    ASSERT_VEC(expected, actual);
}

static void test_vector_scale()
{
    vec v1 = vec_new(1.f, 2.f, 3.f);
    vec actual = vec_scale(v1, 5.f);
    vec expected = vec_new(5.f, 10.f, 15.f);
    ASSERT_VEC(expected, actual);
}

static void test_vector_negate()
{
    vec v1 = vec_new(1.f, 2.f, 3.f);
    vec actual = vec_negate(v1);
    vec expected = vec_new(-1.f, -2.f, -3.f);
    ASSERT_VEC(expected, actual);
}

static void test_vector_normalize()
{
    vec v1 = vec_new(2.f, 3.f, 4.f);
    vec actual = vec_normalize(v1);
    float scale = sqrtf(2 * 2 + 3 * 3 + 4 * 4);
    vec expected = vec_new(2.f/scale, 3.f/scale, 4.f/scale);
    ASSERT_VEC(expected, actual);
}

static void test_vector_dot()
{
    vec v1 = vec_new(1.f, 2.f, 3.f);
    vec v2 = vec_new(3.f, 2.f, 1.f);
    float actual = vec_dot(v1, v2);
    float expected = 10.f;
    ASSERT_FLOAT(expected, actual);
}

static void test_vector_magnitude()
{
    vec v1 = vec_new(2.f, 3.f, 4.f);
    float actual = vec_magnitude(v1);
    float expected = sqrtf(2 * 2 + 3 * 3 + 4 * 4);
    ASSERT_FLOAT(expected, actual);
}

void test_vector()
{
    test_vector_addition();
    test_vector_subtraction();
    test_vector_cross();
    test_vector_scale();
    test_vector_negate();
    test_vector_normalize();
    test_vector_dot();
    test_vector_magnitude();
}