#include "camera.h"

#include <stdlib.h>

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

/********************/
/* public functions */
/********************/

camera_t* camera_new(vec_t position)
{
    camera_t* camera = malloc(sizeof(camera_t));
    camera->position = position;
    camera->forward  = vec_new(0.f, 0.f, 1.f);
    camera->view     = mat_new_identity();

    return camera;
}

void camera_update(camera_t* cam)
{
    // update


    // generate new basis vectors
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

void camera_free(camera_t* cam)
{
    free(cam);
}