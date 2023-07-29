#include "test_clip.h"

#include "test_utils.h"

#include "../clip.h"

static void test_clip_when_all_points_are_inside()
{
    vec_t position = vec_new(0.f, 0.f, 0.f);
    camera_t* camera = camera_new(position, 
                                  0.f, 
                                  0.f, 
                                  90 * (float)M_PI / 180.f,
                                  1.f,
                                  20.f,
                                  1.3333f);

    vec_t v0 = vec_new(0.f, 0.f, 2.f);
    vec_t v1 = vec_new(-0.5f, 0.f, 2.f);
    vec_t v2 = vec_new(0.f, 0.5f, 2.f);

    bool clip = clip_polygon(camera, v0, v1, v2);

    ASSERT_FALSE(clip);

    camera_free(camera);
}

static void test_clip_when_two_points_are_inside()
{
    vec_t position = vec_new(0.f, 0.f, 0.f);
    camera_t* camera = camera_new(position, 
                                  0.f, 
                                  0.f, 
                                  90 * (float)M_PI / 180.f,
                                  1.f,
                                  20.f,
                                  1.3333f);

    vec_t v0 = vec_new(0.f, 0.f, 2.f);
    vec_t v1 = vec_new(-0.5f, 0.f, 2.f);
    vec_t v2 = vec_new(123.f, 0.5f, 2.f);

    bool clip = clip_polygon(camera, v0, v1, v2);

    ASSERT_TRUE(clip);

    camera_free(camera);
}

static void test_clip_when_all_points_are_outside()
{
    vec_t position = vec_new(0.f, 0.f, 0.f);
    camera_t* camera = camera_new(position, 
                                  0.f, 
                                  0.f, 
                                  90 * (float)M_PI / 180.f,
                                  1.f,
                                  20.f,
                                  1.3333f);

    vec_t v0 = vec_new(0.f, 0.f, 40.f);
    vec_t v1 = vec_new(-0.5f, 30.f, 2.f);
    vec_t v2 = vec_new(123.f, 0.5f, 2.f);

    bool clip = clip_polygon(camera, v0, v1, v2);

    ASSERT_TRUE(clip);

    camera_free(camera);
}

void test_clip()
{
    TEST_CASE(test_clip_when_all_points_are_inside);
    TEST_CASE(test_clip_when_two_points_are_inside);
    TEST_CASE(test_clip_when_all_points_are_outside);
}