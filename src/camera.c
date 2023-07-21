#include "camera.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Notes
 * 
 * marsp0.github.io/articles/deriving-camera-transform.html
 *
 */

/********************/
/*      defines     */
/********************/

#define PI_OVER_180 0.017453292f
#define MAX_PITCH   1.55334f

/********************/
/* static variables */
/********************/

/********************/
/* static functions */
/********************/

static void update(camera_t* cam)
{
    vec_t world_up  = vec_new(0.f, 1.f, 0.f);
    
    // generate new basis vectors
    mat_t rot_y_x   = mat_mul_mat(y_axis_rotation(cam->yaw), x_axis_rotation(cam->pitch));
    cam->forward    = vec_normalize(mat_mul_vec(rot_y_x, vec_new(0.f, 0.f, -1.f)));
    cam->side       = vec_normalize(vec_cross(world_up, cam->forward));
    cam->up         = vec_normalize(vec_cross(cam->forward, cam->side));

    // generate new planes
    vec_t view_dir  = vec_negate(cam->forward);

    vec_t n_point   = vec_add(cam->position, vec_scale(view_dir, cam->n_dist));
    vec_t f_point   = vec_add(cam->position, vec_scale(view_dir, cam->f_dist));

    vec_t r_point   = vec_add(n_point, vec_scale(cam->side, cam->r_dist));
    vec_t r_dir     = vec_normalize(vec_sub(r_point, cam->position));

    vec_t l_point   = vec_add(n_point, vec_scale(cam->side, cam->l_dist));
    vec_t l_dir     = vec_normalize(vec_sub(l_point, cam->position));

    vec_t t_point   = vec_add(n_point, vec_scale(cam->up, cam->t_dist));
    vec_t t_dir     = vec_normalize(vec_sub(t_point, cam->position));

    vec_t b_point   = vec_add(n_point, vec_scale(cam->up, cam->b_dist));
    vec_t b_dir     = vec_normalize(vec_sub(b_point, cam->position));

    cam->n_plane.p  = n_point;
    cam->n_plane.n  = view_dir;

    cam->f_plane.p  = f_point;
    cam->f_plane.n  = cam->forward;

    cam->r_plane.p  = r_point;
    cam->r_plane.n  = vec_normalize(vec_cross(cam->up, r_dir));

    cam->l_plane.p  = l_point;
    cam->l_plane.n  = vec_normalize(vec_cross(l_dir, cam->up));

    cam->t_plane.p  = t_point;
    cam->t_plane.n  = vec_normalize(vec_cross(t_dir, cam->side));

    cam->b_plane.p  = b_point;
    cam->b_plane.n  = vec_normalize(vec_cross(cam->side, b_dir));
}

static bool plane_check(plane_t plane, sphere_t sphere)
{
    vec_t pc = vec_sub(sphere.c, plane.p);
    float dot = vec_dot(pc, plane.n);
    if (dot / vec_magnitude(pc) > 0.f)
    {
        return true;
    }

    printf("dist is %f\n", fabs(dot));
    vec_print(plane.n);
    if (fabs(dot) < sphere.r)
    {
        return true;
    }

    return false;
}

/********************/
/* public functions */
/********************/

camera_t* camera_new(vec_t position,
                     float pitch,
                     float yaw,
                     float fov_x,
                     float near,
                     float far,
                     float aspect_ratio)
{
    camera_t* camera    = malloc(sizeof(camera_t));
    
    camera->position    = position;
    camera->pitch       = pitch;
    camera->yaw         = yaw;
    camera->forward     = vec_new(0.f, 0.f, 0.f);
    camera->side        = vec_new(0.f, 0.f, 0.f);
    camera->up          = vec_new(0.f, 0.f, 0.f);

    camera->fov_x       = fov_x;
    camera->asp_ratio   = aspect_ratio;
    camera->n_dist      = near;
    camera->f_dist      = far;
    camera->r_dist      = tanf(fov_x / 2.f) * near;
    camera->l_dist      = -camera->r_dist;
    camera->t_dist      = camera->r_dist / aspect_ratio;
    camera->b_dist      = -camera->t_dist;

    update(camera);

    return camera;
}

void camera_update(camera_t* cam, input_t input)
{
    // origin of xorg window is top-left
    cam->pitch += (float)input.dy * PI_OVER_180;
    cam->yaw   -= (float)input.dx * PI_OVER_180;

    if (cam->pitch > MAX_PITCH || cam->pitch < -MAX_PITCH)
    {
        cam->pitch = cam->pitch > MAX_PITCH ? MAX_PITCH : -MAX_PITCH;
    }

    if (cam->yaw > 2 * M_PI || cam->yaw < -M_PI * 2)
    {
        cam->yaw = 0.f;
    }

    float speed = 0.5f;
    vec_t front = vec_scale(vec_negate(cam->forward), speed);
    vec_t right = vec_scale(cam->side, speed);

    if (input.keys & KEY_W)
    {
        cam->position = vec_add(cam->position, front);
    }
    if (input.keys & KEY_A)
    {
        cam->position = vec_add(cam->position, vec_negate(right));
    }
    if (input.keys & KEY_S)
    {
        cam->position = vec_add(cam->position, vec_negate(front));
    }
    if (input.keys & KEY_D)
    {
        cam->position = vec_add(cam->position, right);
    }

    update(cam);
}

bool camera_is_mesh_visible(camera_t* cam, sphere_t sphere)
{
    return  plane_check(cam->n_plane, sphere) && 
            plane_check(cam->f_plane, sphere) && 
            plane_check(cam->r_plane, sphere) && 
            plane_check(cam->l_plane, sphere) && 
            plane_check(cam->t_plane, sphere) && 
            plane_check(cam->b_plane, sphere);
}

mat_t camera_view_transform(camera_t* cam)
{
    mat_t result = mat_new_identity();

    vec_t pos_negative = vec_negate(cam->position);

    // construct cam -> world transform + invert it
    result.data[0][0] = cam->side.x;
    result.data[0][1] = cam->side.y;
    result.data[0][2] = cam->side.z;
    result.data[0][3] = vec_dot(pos_negative, cam->side);

    result.data[1][0] = cam->up.x;
    result.data[1][1] = cam->up.y;
    result.data[1][2] = cam->up.z;
    result.data[1][3] = vec_dot(pos_negative, cam->up);

    result.data[2][0] = cam->forward.x;
    result.data[2][1] = cam->forward.y;
    result.data[2][2] = cam->forward.z;
    result.data[2][3] = vec_dot(pos_negative, cam->forward);

    return result;
}

mat_t camera_proj_transform(camera_t* cam)
{
    float two_n     = 2 * cam->n_dist;
    float two_f_n   = 2 * cam->n_dist * cam->f_dist;
    float r_min_l   = cam->r_dist - cam->l_dist;
    float r_plus_l  = cam->r_dist + cam->l_dist;
    float t_min_b   = cam->t_dist - cam->b_dist;
    float t_plus_b  = cam->t_dist + cam->b_dist;
    float f_min_n   = cam->f_dist - cam->n_dist;
    float f_plus_n  = cam->f_dist + cam->n_dist;

    mat_t result = mat_new_identity();

    result.data[0][0] = two_n    / r_min_l;
    result.data[0][1] = 0.f;
    result.data[0][2] = r_plus_l / r_min_l;
    result.data[0][3] = 0.f;

    result.data[1][0] = 0.f;
    result.data[1][1] = two_n    / t_min_b;
    result.data[1][2] = t_plus_b / t_min_b;
    result.data[1][3] = 0.f;

    result.data[2][0] = 0.f;
    result.data[2][1] = 0.f;
    result.data[2][2] = -(f_plus_n / f_min_n);
    result.data[2][3] = -(two_f_n  / f_min_n);

    result.data[3][2] = -1.f;
    result.data[3][3] = 0.f;

    return result;
}

void camera_free(camera_t* cam)
{
    free(cam);
}