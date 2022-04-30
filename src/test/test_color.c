#include "test_color.h"

#include "../color.h"
#include "test_utils.h"

void test_color_new_u()
{
    color actual = color_new_u(0, 0, 0);
    color expected = color_new_f(0.f, 0.f, 0.f);
    ASSERT_COL(expected, actual);
}

void test_color_add()
{
    color c1 = color_new_f(0.5f, 0.5f, 0.5f);
    color c2 = color_new_f(0.25f, 0.25f, 0.25f);
    color actual = color_add(c1, c2);
    color expected = color_new_f(0.75f, 0.75f, 0.75f);
    ASSERT_COL(expected, actual);
}

void test_color_mul()
{
    color c1 = color_new_f(0.5f, 0.5f, 0.5f);
    color c2 = color_new_f(0.25f, 0.25f, 0.25f);
    color actual = color_mul(c1, c2);
    color expected = color_new_f(0.125f, 0.125f, 0.125f);
    ASSERT_COL(expected, actual);
}

void test_color_scale()
{
    color c1 = color_new_f(0.5f, 0.5f, 0.5f);
    float scale = 0.5f;
    color actual = color_scale(c1, scale);
    color expected = color_new_f(0.25f, 0.25f, 0.25f);
    ASSERT_COL(expected, actual);
}

void test_color()
{
    test_color_new_u();
    test_color_add();
    test_color_mul();
    test_color_scale();
}