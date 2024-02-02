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

static void diffuse()
{

}

static void specular()
{

}

/********************/
/* public functions */
/********************/

void process_vertex(triangle_t* t, camera_t* c)
{
    mat_t PV = mat_mul_mat(camera_proj_transform(c), camera_view_transform(c));

    // mvp transform
    t->v0 = mat_mul_vec(PV, t->v0);
    t->v1 = mat_mul_vec(PV, t->v1);
    t->v2 = mat_mul_vec(PV, t->v2);
}

uint32_t process_fragment(triangle_t* t, float w0, float w1, float w2)
{
    float u = w0 * t->t0.x + w1 * t->t1.x + w2 * t->t2.x;
    float v = w0 * t->t0.y + w1 * t->t1.y + w2 * t->t2.y;
    uint32_t u_idx = (uint32_t)(u * (float)t->albedo->width);
    uint32_t v_idx = (uint32_t)(v * (float)t->albedo->height);

    diffuse();
    specular();

    return texture_get(t->albedo, u_idx, v_idx);
}