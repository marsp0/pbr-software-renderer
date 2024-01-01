#include "shader.h"

#include <stdio.h>

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

/********************/
/* static functions */
/********************/

/********************/
/* public functions */
/********************/

void process_vertex(triangle_t* t, camera_t* c, vec_t* l, uint32_t l_size)
{
    //TODO REMOVE
    printf("%p, %u\n", (void*)l, l_size);

    mat_t PV = mat_mul_mat(camera_proj_transform(c), camera_view_transform(c));

    // mvp transform
    t->v0 = mat_mul_vec(PV, t->v0);
    t->v1 = mat_mul_vec(PV, t->v1);
    t->v2 = mat_mul_vec(PV, t->v2);
}

void process_fragment()
{

}