#include "test_camera.h"

#include "test_utils.h"

#include "../camera.h"

static void assert_frustum(float pitch, float yaw, vec_t view_dir)
{
    float n_dist        = 1.f;
    float f_dist        = 20.f;
    float fov_x         = deg_to_rad(45.f);
    float asp_ratio     = 1.3333f;
    camera_t* camera    = camera_new(vec_new(0.f, 0.f, 0.f), 
                                     pitch, 
                                     yaw, 
                                     fov_x,
                                     n_dist,
                                     f_dist,
                                     asp_ratio);

    // near
    vec_t expected_n_n  = view_dir;
    vec_t expected_n_p  = vec_scale(expected_n_n, n_dist);

    // far
    vec_t expected_f_n = vec_negate(view_dir);
    vec_t expected_f_p = vec_scale(expected_n_n, f_dist);

    // misc
    // direction vectors relative to view dir (-cam->forward)
    vec_t world_up  = vec_new(0.f, 1.f, 0.f);
    vec_t right     = vec_normalize(vec_cross(expected_n_n, world_up));
    vec_t up        = vec_normalize(vec_cross(right, expected_n_n));

    // right
    // distance from near point on plane to right point on plane
    float r_dist        = tanf(fov_x / 2.f) * n_dist;
    // right point
    vec_t expected_r_p  = vec_add(expected_n_p, vec_scale(right, r_dist));
    // cam position to right point direction
    vec_t cp_rp_dir     = vec_normalize(vec_sub(expected_r_p, camera->position));
    vec_t expected_r_n  = vec_normalize(vec_cross(up, cp_rp_dir));

    // left
    vec_t expected_l_p  = vec_add(expected_n_p, vec_scale(right, -r_dist));
    // cam position to left point direction
    vec_t cp_lp_dir     = vec_normalize(vec_sub(expected_l_p, camera->position));
    vec_t expected_l_n  = vec_normalize(vec_cross(cp_lp_dir, up));

    // top
    // distance from near point on plane to top point on plane
    float t_dist        = r_dist / asp_ratio;
    // top point
    vec_t expected_t_p  = vec_add(expected_n_p, vec_scale(up, t_dist));
    // cam position to top point direction
    vec_t cp_tp_dir     = vec_normalize(vec_sub(expected_t_p, camera->position));
    vec_t expected_t_n  = vec_cross(cp_tp_dir, right);

    // bot
    // distance from near point on plane to bot point on plane
    float b_dist        = -t_dist;
    // bot point
    vec_t expected_b_p  = vec_add(expected_n_p, vec_scale(up, b_dist));
    // cam position to bot point direction
    vec_t cp_bp_dir     = vec_normalize(vec_sub(expected_b_p, camera->position));
    vec_t expected_b_n  = vec_cross(right, cp_bp_dir);

    ASSERT_VECTOR(camera->n_plane.p, expected_n_p);
    ASSERT_VECTOR(camera->n_plane.n, expected_n_n);

    ASSERT_VECTOR(camera->f_plane.p, expected_f_p);
    ASSERT_VECTOR(camera->f_plane.n, expected_f_n);

    ASSERT_VECTOR(camera->r_plane.p, expected_r_p);
    ASSERT_VECTOR(camera->r_plane.n, expected_r_n);

    ASSERT_VECTOR(camera->l_plane.p, expected_l_p);
    ASSERT_VECTOR(camera->l_plane.n, expected_l_n);

    ASSERT_VECTOR(camera->t_plane.p, expected_t_p);
    ASSERT_VECTOR(camera->t_plane.n, expected_t_n);

    ASSERT_VECTOR(camera->b_plane.p, expected_b_p);
    ASSERT_VECTOR(camera->b_plane.n, expected_b_n);

    camera_free(camera);
}

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

static void test_camera_frustum_no_rotation_no_translation()
{
    float pitch = 0.f;
    float yaw = 0.f;
    assert_frustum(pitch, yaw, vec_new(0.f, 0.f, 1.f));
}

static void test_camera_frustum_rotated_90deg_around_y_no_translation()
{
    float pitch = 0.f;
    float yaw = deg_to_rad(90.f);
    assert_frustum(pitch, yaw, vec_new(1.f, 0.f, 0.f));
}

static void test_camera_frustum_rotated_30deg_around_x_no_translation()
{
    float pitch         = deg_to_rad(30.f);
    float yaw           = deg_to_rad(0.f);
    mat_t rot_y_x       = mat_mul_mat(y_axis_rotation(yaw), x_axis_rotation(pitch));
    vec_t cam_forward   = vec_normalize(mat_mul_vec(rot_y_x, vec_new(0.f, 0.f, -1.f)));

    assert_frustum(pitch, yaw, vec_negate(cam_forward));
}

static void test_camera_frustum_rotated_20deg_around_x_and_60_around_y_no_translation()
{
    float pitch         = deg_to_rad(20.f);
    float yaw           = deg_to_rad(60.f);
    mat_t rot_y_x       = mat_mul_mat(y_axis_rotation(yaw), x_axis_rotation(pitch));
    vec_t cam_forward   = vec_normalize(mat_mul_vec(rot_y_x, vec_new(0.f, 0.f, -1.f)));

    assert_frustum(pitch, yaw, vec_negate(cam_forward));
}

static void test_proj_matrix()
{
    vec_t position  = vec_new(0.f, 0.f, 0.f);
    float yaw       = 0.f;
    float pitch     = 0.f;
    float near      = 1.f;
    float far       = 20.f;
    float fov_x     = deg_to_rad(45.f);
    float asp_ratio = 800.f / 600.f;
    float right     = near * tanf(fov_x / 2.f);
    float top       = right / asp_ratio;

    camera_t* camera = camera_new(position,
                                  pitch,
                                  yaw,
                                  fov_x,
                                  near,
                                  far,
                                  asp_ratio);

    mat_t actual = camera_proj_transform(camera);
    mat_t expected = mat_new_identity();
    expected.data[0][0] = near / right;
    expected.data[1][1] = near / top;
    expected.data[2][2] = near / (far - near);
    expected.data[2][3] = near * far / (far - near);
    expected.data[3][2] = -1.f;
    expected.data[3][3] = 0.f;

    ASSERT_MATRIX(actual, expected);

    // near plane point
    position = vec_new(0.f, 0.f, -1.f);
    position = mat_mul_vec(actual, position);
    position = vec_scale(position, 1.f/position.w);
    ASSERT_VECTOR(position, vec_new(0.f, 0.f, 1.f));

    // far plane point
    position = vec_new(0.f, 0.f, -20.f);
    position = mat_mul_vec(actual, position);
    position = vec_scale(position, 1.f/position.w);
    ASSERT_VECTOR(position, vec_new(0.f, 0.f, 0.f));

    camera_free(camera);
}

static void test_camera_visibility_check_when_mesh_is_visible()
{
    vec_t position  = vec_new(0.f, 0.f, 0.f);
    float pitch     = 0.f;
    float yaw       = 0.f;
    float near      = 1.f;
    float far       = 20.f;
    float fov_x     = deg_to_rad(45.f);
    float asp_ratio = 800.f / 600.f;

    camera_t* camera = camera_new(position,
                                  pitch,
                                  yaw,
                                  fov_x,
                                  near,
                                  far,
                                  asp_ratio);

    sphere_t sphere = { .c = vec_new(0.f, 0.f, 5.f),
                        .r = 1.f };

    bool visible = camera_is_mesh_visible(camera, sphere);

    ASSERT_TRUE(visible);

    camera_free(camera);
}

static void test_camera_visibility_check_when_mesh_is_not_visible()
{
    vec_t position  = vec_new(0.f, 0.f, 0.f);
    float pitch     = 0.f;
    float yaw       = 0.f;
    float near      = 1.f;
    float far       = 20.f;
    float fov_x     = deg_to_rad(45.f);
    float asp_ratio = 800.f / 600.f;

    camera_t* camera = camera_new(position,
                                  pitch,
                                  yaw,
                                  fov_x,
                                  near,
                                  far,
                                  asp_ratio);

    sphere_t sphere = { .c = vec_new(0.f, 0.f, -5.f),
                        .r = 1.f };

    bool visible = camera_is_mesh_visible(camera, sphere);

    ASSERT_FALSE(visible);
    ASSERT_TRUE(false);

    camera_free(camera);
}

void test_camera()
{
    TEST_CASE(test_view_matrix_rotated_90deg_around_y);
    TEST_CASE(test_view_matrix_rotated_45deg_around_x);
    TEST_CASE(test_view_matrix_rotated_30deg_around_x_and_45deg_around_y);
    TEST_CASE(test_camera_frustum_no_rotation_no_translation);
    TEST_CASE(test_camera_frustum_rotated_90deg_around_y_no_translation);
    TEST_CASE(test_camera_frustum_rotated_30deg_around_x_no_translation);
    TEST_CASE(test_camera_frustum_rotated_20deg_around_x_and_60_around_y_no_translation);
    TEST_CASE(test_proj_matrix);
    TEST_CASE(test_camera_visibility_check_when_mesh_is_visible);
    TEST_CASE(test_camera_visibility_check_when_mesh_is_not_visible);
}