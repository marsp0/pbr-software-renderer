#include "camera.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Notes
 * 
 * marsp0.github.io/articles/deriving-camera-transform.html
 * 
 *
 * Reverse Z buffer
 * https://developer.nvidia.com/content/depth-precision-visualized
 * http://www.geometry.caltech.edu/pubs/UD12.pdf
 * http://www.humus.name/Articles/Persson_CreatingVastGameWorlds.pdf
 * http://www.terathon.com/gdc07_lengyel.pdf
 * https://www.danielecarbone.com/reverse-depth-buffer-in-opengl/
 * https://stackoverflow.com/questions/63084469/when-and-how-does-opengl-calculate-f-depthdepth-value
 * https://stackoverflow.com/questions/63096579/how-does-opengl-come-to-the-formula-f-depth-and-and-is-this-the-window-viewport
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

static void camera_update_internal(camera_t* cam)
{
    vec4_t world_up  = vec4_new(0.f, 1.f, 0.f);
    
    // generate new basis vectors
    mat_t rot_y_x   = mat_mul_mat(y_axis_rotation(cam->yaw), x_axis_rotation(cam->pitch));
    cam->forward    = vec4_normalize(mat_mul_vec(rot_y_x, vec4_new(0.f, 0.f, -1.f)));
    cam->side       = vec4_normalize(vec4_cross(world_up, cam->forward));
    cam->up         = vec4_normalize(vec4_cross(cam->forward, cam->side));
}

/********************/
/* public functions */
/********************/

camera_t* camera_new(vec4_t position,
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
    camera->forward     = vec4_new(0.f, 0.f, 0.f);
    camera->side        = vec4_new(0.f, 0.f, 0.f);
    camera->up          = vec4_new(0.f, 0.f, 0.f);

    camera->fov_x       = fov_x;
    camera->asp_ratio   = aspect_ratio;
    camera->n_dist      = near;
    camera->f_dist      = far;
    camera->r_dist      = tanf(fov_x / 2.f) * near;
    camera->l_dist      = -camera->r_dist;
    camera->t_dist      = camera->r_dist / aspect_ratio;
    camera->b_dist      = -camera->t_dist;

    camera_update_internal(camera);

    return camera;
}

void camera_update(camera_t* cam, input_t input, float dt)
{
    // origin of xorg window is top-left
    cam->pitch += deg_to_rad((float)input.dy);
    cam->yaw   -= deg_to_rad((float)input.dx);

    if (cam->pitch > MAX_PITCH || cam->pitch < -MAX_PITCH)
    {
        cam->pitch = cam->pitch > MAX_PITCH ? MAX_PITCH : -MAX_PITCH;
    }

    if (cam->yaw > 2 * M_PI || cam->yaw < -M_PI * 2)
    {
        cam->yaw = 0.f;
    }

    vec4_t front = vec4_negate(cam->forward);
    vec4_t right = cam->side;
    vec4_t position = cam->position;
    vec4_t dir;

    if (input.keys & KEY_W)
    {
        dir = front;
    }
    if (input.keys & KEY_A)
    {
        dir = vec4_negate(right);
    }
    if (input.keys & KEY_S)
    {
        dir = vec4_negate(front);
    }
    if (input.keys & KEY_D)
    {
        dir = right;
    }

    cam->position = vec4_add(position, vec4_scale(dir, dt));

    camera_update_internal(cam);
}

mat_t camera_view_transform(camera_t* cam)
{
    mat_t result = mat_new_identity();

    vec4_t pos_negative = vec4_negate(cam->position);

    // construct cam -> world transform + invert it
    result.data[0][0] = cam->side.x;
    result.data[0][1] = cam->side.y;
    result.data[0][2] = cam->side.z;
    result.data[0][3] = vec4_dot(pos_negative, cam->side);

    result.data[1][0] = cam->up.x;
    result.data[1][1] = cam->up.y;
    result.data[1][2] = cam->up.z;
    result.data[1][3] = vec4_dot(pos_negative, cam->up);

    result.data[2][0] = cam->forward.x;
    result.data[2][1] = cam->forward.y;
    result.data[2][2] = cam->forward.z;
    result.data[2][3] = vec4_dot(pos_negative, cam->forward);

    return result;
}

mat_t camera_proj_transform(camera_t* cam)
{
    mat_t result = mat_new_identity();

    float n_over_r  = cam->n_dist / cam->r_dist;
    float n_over_t  = cam->n_dist / cam->t_dist;
    float f_min_n   = cam->f_dist - cam->n_dist;
    float n         = cam->n_dist;
    float fn        = cam->n_dist * cam->f_dist;

    result.data[0][0] = n_over_r;
    result.data[1][1] = n_over_t;
    result.data[2][2] = n / f_min_n;
    result.data[2][3] = fn / f_min_n;
    result.data[3][2] = -1.f;
    result.data[3][3] = 0.f;

    return result;
}

void camera_free(camera_t* cam)
{
    free(cam);
}
