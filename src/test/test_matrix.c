#include "test_matrix.h"

#include "test_utils.h"

static void test_matrix_add()
{
    mat_t m1            = mat_new_identity();
    mat_t m2            = mat_new_identity();
    mat_t actual        = mat_add(m1, m2);
    mat_t expected      = mat_new_identity();
    expected.data[0][0] = 2.f;
    expected.data[1][1] = 2.f;
    expected.data[2][2] = 2.f;
    expected.data[3][3] = 2.f;

    ASSERT_MATRIX(expected, actual);
}

static void test_matrix_sub()
{
    mat_t m1            = mat_new_identity();
    mat_t m2            = mat_new_identity();
    mat_t actual        = mat_sub(m1, m2);
    mat_t expected      = mat_new_identity();
    expected.data[0][0] = 0.f;
    expected.data[1][1] = 0.f;
    expected.data[2][2] = 0.f;
    expected.data[3][3] = 0.f;

    ASSERT_MATRIX(expected, actual);
}

static void test_matrix_mul_matrix()
{
    mat_t m1        = mat_new(5, 2, 3, 4, 0, 1, 0, 0, 16, 12, 1, 0, 3, 4, 0, 1);
    mat_t m2        = mat_new(1, 1, 0, 1, 6, 1, 0, 3, 2, 1, 14, 0, 4, 0, 2, 1);
    mat_t actual    = mat_mul_mat(m1, m2);
    mat_t expected  = mat_new(39, 10, 50, 15, 6, 1, 0, 3, 90, 29, 14, 52, 31, 7, 2, 16);

    ASSERT_MATRIX(expected, actual);
}

static void test_matrix_mul_vector()
{
    mat_t m         = mat_new(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0, 0, 0, 1);
    vec4_t v        = vec4_new(1, 2, 3);
    vec4_t actual   = mat_mul_vec(m, v);
    vec4_t expected = vec4_new(18.f, 46.f, 74.f);

    ASSERT_VECTOR4(expected, actual);
}

static void test_matrix_inverse_1()
{
    mat_t m         = mat_new(1, 1, 1, -1, 1, 1, -1, 1, 1, -1, 1, 1, -1, 1, 1, 1);
    mat_t actual    = mat_inverse(m);
    mat_t expected  = mat_new(0.25f,0.25f,0.25f,-0.25f, 
                              0.25f,0.25f,-0.25f,0.25f,
                              0.25f,-0.25f,0.25f,0.25f,
                             -0.25f,0.25f,0.25f,0.25f);

    ASSERT_MATRIX(expected, actual);
}

static void test_matrix_inverse_2()
{
    mat_t m         = mat_new(1,2,3,4,
                              5,6,7,8,
                              9,1,3,3,
                              4,5,6,6);
    mat_t actual    = mat_inverse(m);
    mat_t expected  = mat_new(-9.f/40.f, 9/40.f, 1.f/10.f, -1.f/5.f, 
                              -21.f/20.f, 21/20.f, -1.f/5.f, -3.f/5.f,
                               31.f/40.f, -71.f/40.f, 1.f/10.f, 9.f/5.f,
                               1.f/4.f, 3.f/4.f, 0.f, -1.f);

    ASSERT_MATRIX(expected, actual);
}

static void test_matrix_inverse_3()
{
    mat_t m         = mat_new(0.5f,2,3,4,
                              5,2,7,8,
                              9,2,3,11,
                              4,5,6,6);
    mat_t actual    = mat_inverse(m);
    mat_t expected  = mat_new(-0.43249f, 0.05017f, 0.04778f, 0.13381f,
                              0.06212f, -0.27240f, 0.02628f, 0.27359f,
                             -0.14575f, 0.25448f, -0.13859f, 0.01194f,
                              0.38231f, -0.06093f, 0.08482f, -0.16248f);

    ASSERT_MATRIX(expected, actual);
}

static void test_matrix_transpose()
{
    mat_t m         = mat_new(1,2,3,4,
                              5,6,7,8,
                              9,10,11,12,
                              13,14,15,16);
    mat_t actual    = mat_transpose(m);
    mat_t expected  = mat_new(1,5,9,13,
                              2,6,10,14,
                              3,7,11,15,
                              4,8,12,16);

    ASSERT_MATRIX(expected, actual);
}

static void test_x_axis_rotation()
{
    mat_t r         = x_axis_rotation(deg_to_rad(90.f));
    vec4_t v        = vec4_new(0.f, 0.f, 1.f);

    vec4_t actual   = mat_mul_vec(r, v);

    ASSERT_VECTOR4(vec4_new(0.f, -1.f, 0.f), actual);
}

static void test_y_axis_rotation()
{
    mat_t r         = y_axis_rotation(deg_to_rad(90.f));
    vec4_t v        = vec4_new(0.f, 0.f, 1.f);

    vec4_t actual   = mat_mul_vec(r, v);

    ASSERT_VECTOR4(vec4_new(1.f, 0.f, 0.f), actual);
}

static void test_z_axis_rotation()
{
    mat_t r         = z_axis_rotation(deg_to_rad(90.f));
    vec4_t v        = vec4_new(1.f, 0.f, 0.f);

    vec4_t actual   = mat_mul_vec(r, v);

    ASSERT_VECTOR4(vec4_new(0.f, 1.f, 0.f), actual);
}

static void test_deg_to_rad()
{
    float deg       = 35.f;
    float actual    = deg_to_rad(deg);

    ASSERT_EQUAL(0.6108652382f, actual);

    deg             = 130.f;
    actual          = deg_to_rad(deg);

    ASSERT_EQUAL(2.2689280276f, actual);

    deg             = 250.f;
    actual          = deg_to_rad(deg);

    ASSERT_EQUAL(4.36332313f, actual);

    deg             = 340.f;
    actual          = deg_to_rad(deg);

    ASSERT_EQUAL(5.9341194568f, actual);

    deg             = 390.f;
    actual          = deg_to_rad(deg);

    ASSERT_EQUAL(6.8067840828f, actual);
}

static void test_rad_to_deg()
{
    float rad       = 0.6108652382f;
    float actual    = rad_to_deg(rad);

    ASSERT_EQUAL(35.f, actual);

    rad             = 2.2689280276f;
    actual          = rad_to_deg(rad);

    ASSERT_EQUAL(130.f, actual);

    rad             = 4.36332313f;
    actual          = rad_to_deg(rad);

    ASSERT_EQUAL(250.f, actual);

    rad             = 5.9341194568f;
    actual          = rad_to_deg(rad);

    ASSERT_EQUAL(340.f, actual);

    rad             = 6.8067840828f;
    actual          = rad_to_deg(rad);

    ASSERT_EQUAL(390.f, actual);
}

static void test_mat_from_vec4()
{
    vec4_t v1 = vec4_new(1.f, 2.f, 3.f);
    vec4_t v2 = vec4_new(11.f, 22.f, 33.f);
    vec4_t v3 = vec4_new(111.f, 222.f, 333.f);

    v1.w = 4.f;

    mat_t actual = mat_from_vec4(v1, v2, v3);
    mat_t expected = mat_new(1.f,   11.f,   111.f,  0.f,
                             2.f,   22.f,   222.f,  0.f,
                             3.f,   33.f,   333.f,  0.f,
                             0.f,   0.f,    0.f,    1.f);

    ASSERT_MATRIX(expected, actual);
}

void test_matrix()
{
    TEST_CASE(test_matrix_add);
    TEST_CASE(test_matrix_sub);
    TEST_CASE(test_matrix_mul_matrix);
    TEST_CASE(test_matrix_mul_vector);
    TEST_CASE(test_matrix_inverse_1);
    TEST_CASE(test_matrix_inverse_2);
    TEST_CASE(test_matrix_inverse_3);
    TEST_CASE(test_matrix_transpose);
    TEST_CASE(test_x_axis_rotation);
    TEST_CASE(test_y_axis_rotation);
    TEST_CASE(test_z_axis_rotation);
    TEST_CASE(test_deg_to_rad);
    TEST_CASE(test_rad_to_deg);
    TEST_CASE(test_mat_from_vec4);
}