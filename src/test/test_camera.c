#include "test_camera.h"

#include "test_utils.h"

#include "../camera.h"

static void test_view_matrix_rotated_90deg_around_y()
{
    // 90 deg around y results in negative rotation 
    // since pitch is *subtracted* from the accumulated pitch
    input_t input   = { .keys = 0, .dx = 90.f, .dy = 0 };
    vec_t forward   = vec_new(0.f, 0.f, -1.f);
    vec_t position  = vec_new(0.f, 0.f, 0.f);
    vec_t new_pos   = vec_new(10.f, 11.f, 12.f);
    vec_t new_left  = vec_new(0.f, 0.f, -1.f);
    vec_t new_up    = vec_new(0.f, 1.f, 0.f);
    vec_t new_front = vec_new(1.f, 0.f, 0.f);
    mat_t view      = mat_new_identity();

    // create camera -> 
    // left gets calculated to      (-1, 0, 0)
    // up gets calculated to        (0, 1, 0)
    // forward gets calculated to   (0, 0, -1)
    camera_t* camera = camera_new(position,
                                  0.f,
                                  0.f,
                                  (float)M_PI_2, 
                                  1.f, 
                                  100.f, 
                                  1.3333f);

    // assert initial values
    ASSERT_VECTOR(position, camera->position);
    ASSERT_VECTOR(forward, camera->forward);

    view.data[0][0] = -1.f;
    view.data[2][2] = -1.f;
    ASSERT_MATRIX(view, camera_view_transform(camera));

    // move camera
    camera->position = new_pos;
    camera_update(camera, input);
    
    // assert view transform
    view.data[0][0] = new_left.x;
    view.data[0][1] = new_left.y;
    view.data[0][2] = new_left.z;
    view.data[0][3] = vec_dot(vec_negate(new_pos), new_left);

    view.data[1][0] = new_up.x;
    view.data[1][1] = new_up.y;
    view.data[1][2] = new_up.z;
    view.data[1][3] = vec_dot(vec_negate(new_pos), new_up);

    view.data[2][0] = new_front.x;
    view.data[2][1] = new_front.y;
    view.data[2][2] = new_front.z;
    view.data[2][3] = vec_dot(vec_negate(new_pos), new_front);

    ASSERT_MATRIX(view, camera_view_transform(camera));

    camera_free(camera);
}

static void test_view_matrix_rotated_45deg_around_x()
{
    input_t input   = { .keys = 0, .dx = 0, .dy = 45.f };
    vec_t position  = vec_new(0.f, 0.f, 0.f);
    vec_t forward   = vec_new(0.f, 0.f, -1.f);
    vec_t new_pos   = vec_new(10.f, 11.f, 12.f);
    vec_t new_left  = vec_normalize(vec_new(-0.7071f, 0.f, 0.f));
    vec_t new_up    = vec_normalize(vec_new(0.f, 0.5f, 0.5f));
    vec_t new_front = vec_normalize(vec_new(0.f, 0.7071f, -0.7071f));
    mat_t view      = mat_new_identity();

    // create camera
    camera_t* camera = camera_new(position, 
                                  0.f,
                                  0.f,
                                  (float)M_PI_2, 
                                  1.f, 
                                  100.f, 
                                  1.3333f);

    // assert initial values
    ASSERT_VECTOR(position, camera->position);
    ASSERT_VECTOR(forward, camera->forward);

    view.data[0][0] = -1.f;
    view.data[2][2] = -1.f;
    ASSERT_MATRIX(view, camera_view_transform(camera));

    // move camera
    camera->position = new_pos;
    camera_update(camera, input);
    
    // assert view transform
    view.data[0][0] = new_left.x;
    view.data[0][1] = new_left.y;
    view.data[0][2] = new_left.z;
    view.data[0][3] = vec_dot(vec_negate(new_pos), new_left);

    view.data[1][0] = new_up.x;
    view.data[1][1] = new_up.y;
    view.data[1][2] = new_up.z;
    view.data[1][3] = vec_dot(vec_negate(new_pos), new_up);

    view.data[2][0] = new_front.x;
    view.data[2][1] = new_front.y;
    view.data[2][2] = new_front.z;
    view.data[2][3] = vec_dot(vec_negate(new_pos), new_front);

    ASSERT_MATRIX(view, camera_view_transform(camera));

    camera_free(camera);
}

static void test_view_matrix_rotated_30deg_around_x_and_45deg_around_y()
{
    input_t input   = { .keys = 0, .dx = -45, .dy = 30 };
    vec_t position  = vec_new(0.f, 0.f, 0.f);
    vec_t forward   = vec_new(0.f, 0.f, -1.f);
    vec_t new_pos   = vec_new(10.f, 11.f, 12.f);
    vec_t new_left  = vec_normalize(vec_new(-0.61237f, 0.f, 0.61237f));
    vec_t new_up    = vec_normalize(vec_new(0.306185f, 0.75f, 0.306185f));
    vec_t new_front = vec_normalize(vec_new(-0.61237f, 0.5f, -0.61237f));
    mat_t view      = mat_new_identity();

    // create camera
    camera_t* camera = camera_new(position, 
                                  0.f,
                                  0.f,
                                  (float)M_PI_2, 
                                  1.f, 
                                  100.f, 
                                  1.3333f);

    // assert initial values
    ASSERT_VECTOR(position, camera->position);
    ASSERT_VECTOR(forward, camera->forward);

    view.data[0][0] = -1.f;
    view.data[2][2] = -1.f;
    ASSERT_MATRIX(view, camera_view_transform(camera));

    // move camera
    camera->position = new_pos;
    camera_update(camera, input);
    
    // assert view transform
    view.data[0][0] = new_left.x;
    view.data[0][1] = new_left.y;
    view.data[0][2] = new_left.z;
    view.data[0][3] = vec_dot(vec_negate(new_pos), new_left);

    view.data[1][0] = new_up.x;
    view.data[1][1] = new_up.y;
    view.data[1][2] = new_up.z;
    view.data[1][3] = vec_dot(vec_negate(new_pos), new_up);

    view.data[2][0] = new_front.x;
    view.data[2][1] = new_front.y;
    view.data[2][2] = new_front.z;
    view.data[2][3] = vec_dot(vec_negate(new_pos), new_front);

    ASSERT_MATRIX(view, camera_view_transform(camera));

    camera_free(camera);
}

void test_camera()
{
    TEST_CASE(test_view_matrix_rotated_90deg_around_y);
    TEST_CASE(test_view_matrix_rotated_45deg_around_x);
    TEST_CASE(test_view_matrix_rotated_30deg_around_x_and_45deg_around_y);
}