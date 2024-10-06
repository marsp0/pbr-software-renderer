#include "shader.h"

/********************
 *  Notes
 *
 ********************/

/********************/
/*      defines     */
/********************/

/********************/
/* static variables */
/********************/

static camera_t* cam = NULL;

/********************/
/* static functions */
/********************/

/********************/
/* public functions */
/********************/


void shader_set_camera(camera_t* camera)
{
    cam = camera;
}

vec4_t shader_vertex(vec4_t v)
{
    mat_t PV = mat_mul_mat(camera_proj_transform(cam),
                           camera_view_transform(cam));

    return mat_mul_vec(PV, v);
}

uint32_t shader_fragment()
{

}