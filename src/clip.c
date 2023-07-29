#include "clip.h"

#include <string.h>
#include <stdio.h>

/********************
 *  Notes
 * 
 *  Algorithm from Wikipedia: https://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm
 *
 *  List outputList = subjectPolygon;  
 *
 *  for (Edge clipEdge in clipPolygon) do
 *      List inputList = outputList;
 *      outputList.clear();
 *
 *      for (int i = 0; i < inputList.count; i += 1) do
 *          Point current_point = inputList[i];
 *          Point prev_point = inputList[(i − 1) % inputList.count];
 *
 *          Point Intersecting_point = ComputeIntersection(prev_point, current_point, clipEdge)
 *
 *          if (current_point inside clipEdge) then
 *              if (prev_point not inside clipEdge) then
 *                  outputList.add(Intersecting_point);
 *              end if
 *              outputList.add(current_point);
 *
 *          else if (prev_point inside clipEdge) then
 *              outputList.add(Intersecting_point);
 *          end if
 *
 *      done
 *  done
 * 
 * 
 * PREV    CURR    -> 
 *  x       x      -> current
 *  x       -      -> intersection
 *  -       x      -> intersection + current
 * 
 ********************/

/********************/
/*      defines     */
/********************/

// #define MAX_POINTS 10

/********************/
/* static variables */
/********************/

/********************/
/* static functions */
/********************/

// static void clip(plane_t plane, vec_t* input, vec_t* output, uint32_t* o_size)
// {
//     // copy output list into input list
//     memcpy(input, output, 10 * sizeof(vec_t));
//     uint32_t input_size = *o_size;
//     uint32_t output_size = 0;

//     vec_t p0 = plane.p;
//     vec_t n  = plane.n;

//     for (uint32_t i = 0; i < input_size; i++)
//     {
//         vec_t curr      = input[i];
//         vec_t prev      = input[(i - 1) % input_size];
//         vec_t p0_curr   = vec_normalize(vec_sub(curr, p0));
//         vec_t p0_prev   = vec_normalize(vec_sub(prev, p0));
//         vec_t l         = vec_normalize(vec_sub(curr, prev));
//         vec_t curr_p0   = vec_sub(p0, curr);

//         bool prev_in    = vec_dot(p0_prev, n) > 0.f;
//         bool curr_in    = vec_dot(p0_curr, n) > 0.f;
//         float l_dot_n   = vec_dot(l, n);
//         float d         = 0.f;

//         // both vertices are outside
//         if (!prev_in && !curr_in)
//         {
//             continue;
//         }

//         if (prev_in ^ curr_in)
//         {
//             d                       = vec_dot(curr_p0, n) / l_dot_n;
//             output[output_size]   = vec_add(prev, vec_scale(l, d));
//             output_size++;
//         }

//         if (curr_in)
//         {
//             output[output_size] = curr;
//             output_size++;
//         }
//     }

//     *o_size = output_size;
// }

static bool is_outside(plane_t plane, vec_t v0, vec_t v1, vec_t v2)
{
    vec_t p0v0 = vec_normalize(vec_sub(v0, plane.p));
    vec_t p0v1 = vec_normalize(vec_sub(v1, plane.p));
    vec_t p0v2 = vec_normalize(vec_sub(v2, plane.p));

    float dot0 = vec_dot(p0v0, plane.n);
    float dot1 = vec_dot(p0v1, plane.n);
    float dot2 = vec_dot(p0v2, plane.n);

    if (dot0 < 0.f || dot1 < 0.f || dot2 < 0.f)
    {
        return true;
    }

    return false;
}

/********************/
/* public functions */
/********************/

bool clip_polygon(camera_t* camera, vec_t v0, vec_t v1, vec_t v2)
{
    return  is_outside(camera->n_plane, v0, v1, v2) ||
            is_outside(camera->f_plane, v0, v1, v2) ||
            is_outside(camera->r_plane, v0, v1, v2) ||
            is_outside(camera->l_plane, v0, v1, v2) ||
            is_outside(camera->t_plane, v0, v1, v2) ||
            is_outside(camera->b_plane, v0, v1, v2);
}