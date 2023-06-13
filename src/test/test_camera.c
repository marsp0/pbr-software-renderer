#include "test_camera.h"

#include "test_utils.h"

#include "../camera.h"

static void test_view_matrix_rotated_90deg_around_y()
{
    vec_t position  = vec_new(0.f, 0.f, 0.f);
    vec_t forward   = vec_new(0.f, 0.f, 1.f);
    vec_t new_pos   = vec_new(10.f, 11.f, 12.f);
    vec_t new_left  = vec_new(0.f, 0.f, 1.f);
    vec_t new_up    = vec_new(0.f, 1.f, 0.f);
    vec_t new_front = vec_new(-1.f, 0.f, 0.f);
    mat_t view      = mat_new_identity();

    // create camera
    camera_t* camera = camera_new(position);

    // assert initial values
    ASSERT_VEC(position, camera->position);
    ASSERT_VEC(forward, camera->forward);
    ASSERT_MAT(view, camera->view);

    // move camera
    // TODO: below changes with call to camera_update with some parameters
    camera->forward = vec_new(1.f, 0.f, 0.f);
    camera->position = new_pos;
    camera_update(camera);
    
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

    ASSERT_MAT(view, camera->view);
}

static void test_view_matrix_rotated_45deg_around_x()
{
    vec_t position  = vec_new(0.f, 0.f, 0.f);
    vec_t forward   = vec_new(0.f, 0.f, 1.f);
    vec_t new_pos   = vec_new(10.f, 11.f, 12.f);
    vec_t new_left  = vec_new(-0.7071f, 0.f, 0.f);
    vec_t new_up    = vec_new(0.f, 0.5f, 0.5f);
    vec_t new_front = vec_new(0.f, 0.7071f, -0.7071f);
    mat_t view      = mat_new_identity();

    // create camera
    camera_t* camera = camera_new(position);

    // assert initial values
    ASSERT_VEC(position, camera->position);
    ASSERT_VEC(forward, camera->forward);
    ASSERT_MAT(view, camera->view);

    // move camera
    // TODO: below changes with call to camera_update with some parameters
    camera->forward = vec_new(0.f, -0.7071f, 0.7071f);
    camera->position = new_pos;
    camera_update(camera);
    
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

    ASSERT_MAT(view, camera->view);
}

static void test_view_matrix_rotated_30deg_around_x_and_45deg_around_y()
{
    vec_t position  = vec_new(0.f, 0.f, 0.f);
    vec_t forward   = vec_new(0.f, 0.f, 1.f);
    vec_t new_pos   = vec_new(10.f, 11.f, 12.f);
    vec_t new_left  = vec_new(-0.61237f, 0.f, 0.61237f);
    vec_t new_up    = vec_new(0.306185f, 0.75f, 0.306185f);
    vec_t new_front = vec_new(-0.61237f, 0.5f, -0.61237f);
    mat_t view      = mat_new_identity();

    // create camera
    camera_t* camera = camera_new(position);

    // assert initial values
    ASSERT_VEC(position, camera->position);
    ASSERT_VEC(forward, camera->forward);
    ASSERT_MAT(view, camera->view);

    // move camera
    // TODO: below changes with call to camera_update with some parameters
    camera->forward = vec_new(0.61237f, -0.5f, 0.61237f);
    camera->position = new_pos;
    camera_update(camera);
    
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

    ASSERT_MAT(view, camera->view);
}

void test_camera()
{
    test_view_matrix_rotated_90deg_around_y();
    test_view_matrix_rotated_45deg_around_x();
    test_view_matrix_rotated_30deg_around_x_and_45deg_around_y();
}