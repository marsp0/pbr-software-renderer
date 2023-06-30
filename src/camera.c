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

/********************/
/* static variables */
/********************/

/********************/
/* static functions */
/********************/

static void update_view_transform(camera_t* cam)
{
    vec_t world_up      = vec_new(0.f, 1.f, 0.f);
    vec_t front         = vec_negate(cam->forward);
    vec_t left          = vec_cross(world_up, front);
    vec_t up            = vec_cross(front, left);
    vec_t pos_negative  = vec_negate(cam->position);

    // construct cam -> world transform + invert it
    cam->view.data[0][0] = left.x;
    cam->view.data[0][1] = left.y;
    cam->view.data[0][2] = left.z;
    cam->view.data[0][3] = vec_dot(pos_negative, left);

    cam->view.data[1][0] = up.x;
    cam->view.data[1][1] = up.y;
    cam->view.data[1][2] = up.z;
    cam->view.data[1][3] = vec_dot(pos_negative, up);

    cam->view.data[2][0] = front.x;
    cam->view.data[2][1] = front.y;
    cam->view.data[2][2] = front.z;
    cam->view.data[2][3] = vec_dot(pos_negative, front);
}

static void update_projection_transform(camera_t* cam)
{
    float two_n     = 2 * cam->near_dist;
    float two_f_n   = 2 * cam->near_dist * cam->far_dist;
    float r_min_l   = cam->right_dist - cam->left_dist;
    float r_plus_l  = cam->right_dist + cam->left_dist;
    float t_min_b   = cam->top_dist - cam->bottom_dist;
    float t_plus_b  = cam->top_dist + cam->bottom_dist;
    float f_min_n   = cam->far_dist - cam->near_dist;
    float f_plus_n  = cam->far_dist + cam->near_dist;

    cam->proj.data[0][0] = two_n    / r_min_l;
    cam->proj.data[0][1] = 0.f;
    cam->proj.data[0][2] = r_plus_l / r_min_l;
    cam->proj.data[0][3] = 0.f;

    cam->proj.data[1][0] = 0.f;
    cam->proj.data[1][1] = two_n    / t_min_b;
    cam->proj.data[1][2] = t_plus_b / t_min_b;
    cam->proj.data[1][3] = 0.f;

    cam->proj.data[2][0] = 0.f;
    cam->proj.data[2][1] = 0.f;
    cam->proj.data[2][2] = -(f_plus_n / f_min_n);
    cam->proj.data[2][3] = two_f_n    / f_min_n;

    cam->proj.data[3][2] = -1.f;
    cam->proj.data[3][3] = 0.f;
}

/********************/
/* public functions */
/********************/

camera_t* camera_new(vec_t position,
                     vec_t forward,
                     float fov_x,
                     float near,
                     float far,
                     float aspect_ratio)
{
    camera_t* camera        = malloc(sizeof(camera_t));
    camera->position        = position;
    camera->forward         = vec_normalize(forward);
    camera->view            = mat_new_identity();
    camera->proj            = mat_new_identity();
    camera->fov_x           = fov_x;
    camera->near_dist   = near;
    camera->far_dist    = far;
    camera->right_dist  = tanf(fov_x / 2.f) * near;
    camera->left_dist   = -camera->right_dist;
    camera->top_dist    = camera->right_dist / aspect_ratio;
    camera->bottom_dist = -camera->top_dist;

    return camera;
}

void camera_update(camera_t* cam)
{
    // update


    update_view_transform(cam);
    update_projection_transform(cam);
}

void camera_free(camera_t* cam)
{
    free(cam);
}