#include "shader.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/********************
 *  Notes
 *
 * - disney pbr paper   - https://media.disneyanimation.com/uploads/production/publication_asset/48/asset/s2012_pbs_disney_brdf_notes_v3.pdf
 * - unreal pbr paper   - https://cdn2.unrealengine.com/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf
 * - LearnOpengl PBR    - https://learnopengl.com/PBR/Theory
 * - Specular BRDF ref  - http://graphicrants.blogspot.nl/2013/08/specular-brdf-reference.html
 * - Tangent space      - https://learnopengl.com/Advanced-Lighting/Normal-Mapping
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

static vec4_t vec4_mix(vec4_t v0, vec4_t v1, vec4_t mix)
{
    float x = v0.x * (1 - mix.x) + v1.x * mix.x;
    float y = v0.y * (1 - mix.y) + v1.y * mix.y;
    float z = v0.z * (1 - mix.z) + v1.z * mix.z;

    return vec4_new(x, y, z);
}

static float schlick_ggx(float dot, float k)
{
    return dot / (dot * (1 - k) + k);
}

// Distribution function describing how much microfacet normals are aligned to the half vector
static float normal_dist(float n_dot_h, float r)
{
    // Trowbridge-Reitz GGX
    float r_sq_sq   = r * r * r * r;
    float b         = n_dot_h * n_dot_h * (r_sq_sq - 1) + 1;
    float denom     = F_PI * b * b;
    float result    = r_sq_sq / denom;

    return result;
}

// Geometry function describing self shadowing
static float self_shadow(float n_dot_v, float n_dot_l, float r)
{
    // Smith + Schlick-GGX
    float k_direct  = (r + 1) * (r + 1) / 8;
    // float k_ibl     = a * a / 2
    float ggx_1     = schlick_ggx(n_dot_v, k_direct);
    float ggx_2     = schlick_ggx(n_dot_l, k_direct);
    float result    = ggx_1 * ggx_2;

    return result;
}

// Fresnel factor describing ratio of reflected vs refracted light
static vec4_t fresnel(float n_dot_h, vec4_t a, vec4_t m)
{
    // Fresnel-Schlick approx
    vec4_t f0       = vec4_from_scalar(0.04f);
    f0              = vec4_mix(f0, a, m);
    vec4_t one      = vec4_new(1.f, 1.f, 1.f);
    vec4_t diff     = vec4_sub(one, f0);
    float exp       = (float)pow(f_clamp(1.f - n_dot_h, 0.f, 1.f), 5.f);
    diff            = vec4_scale(diff, exp);
    vec4_t result   = vec4_add(f0, diff);

    return result;
}

static uint32_t sample(texture_t* tex, float u, float v)
{
    uint32_t u_idx = (uint32_t)(u * (float)tex->width) - 1;
    uint32_t v_idx = (uint32_t)(v * (float)tex->height) - 1;

    return texture_get(tex, u_idx, v_idx);
}

/********************/
/* public functions */
/********************/

void shader_set_camera(camera_t* c)
{
    cam = c;
}

void shader_process_triangle(triangle_t* tri)
{
    vec4_t v0       = tri->v0;
    vec4_t v1       = tri->v1;
    vec4_t v2       = tri->v2;

    vec2_t t0       = tri->t0;
    vec2_t t1       = tri->t1;
    vec2_t t2       = tri->t2;
    
    vec4_t n0       = tri->n0;
    vec4_t n1       = tri->n1;
    vec4_t n2       = tri->n2;

    vec4_t cam_W    = cam->position;

    mat_t M         = mat_new_identity();
    mat_t V         = camera_view_transform(cam);
    mat_t P         = camera_proj_transform(cam);
    mat_t VM        = mat_mul_mat(V, M);
    mat_t PVM       = mat_mul_mat(P, VM);
    mat_t N         = mat_inverse(mat_transpose(M));
    
    vec4_t v0_W     = mat_mul_vec(M, v0);
    vec4_t v1_W     = mat_mul_vec(M, v1);
    vec4_t v2_W     = mat_mul_vec(M, v2);

    // compute tangent/bitangent
    vec2_t d_uv1    = vec2_sub(t1, t0);
    vec2_t d_uv2    = vec2_sub(t2, t0);
    vec4_t e1       = vec4_sub(v1, v0);
    vec4_t e2       = vec4_sub(v2, v0);
    float scale     = 1.f / (d_uv1.x * d_uv2.y - d_uv2.x * d_uv1.y);    // denom for inverse of uv mat
    float tx        = (e1.x * d_uv2.y - e2.x * d_uv1.y) * scale;
    float ty        = (e1.y * d_uv2.y - e2.y * d_uv1.y) * scale;
    float tz        = (e1.z * d_uv2.y - e2.z * d_uv1.y) * scale;
    vec4_t tan      = vec4_normalize(vec4_new(tx, ty, tz));

    // get normal/tangent/bitangent in world space
    vec4_t n0_W     = vec4_normalize(mat_mul_vec(N, n0));
    vec4_t n1_W     = vec4_normalize(mat_mul_vec(N, n1));
    vec4_t n2_W     = vec4_normalize(mat_mul_vec(N, n2));

    vec4_t tan0_W   = mat_mul_vec(N, vec4_sub(tan, vec4_scale(n0, vec4_dot(n0, tan))));
    vec4_t tan1_W   = mat_mul_vec(N, vec4_sub(tan, vec4_scale(n1, vec4_dot(n1, tan))));
    vec4_t tan2_W   = mat_mul_vec(N, vec4_sub(tan, vec4_scale(n2, vec4_dot(n2, tan))));
    
    vec4_t btan0_W  = vec4_normalize(vec4_cross(tan0_W, n0_W));
    vec4_t btan1_W  = vec4_normalize(vec4_cross(tan1_W, n1_W));
    vec4_t btan2_W  = vec4_normalize(vec4_cross(tan2_W, n2_W));
    
    mat_t tbn0_W_T  = mat_transpose(mat_from_vec4(tan0_W, btan0_W, n0_W));
    mat_t tbn1_W_T  = mat_transpose(mat_from_vec4(tan1_W, btan1_W, n1_W));
    mat_t tbn2_W_T  = mat_transpose(mat_from_vec4(tan2_W, btan2_W, n2_W));

    // set view directions
    tri->view0_T    = vec4_normalize(mat_mul_vec(tbn0_W_T, vec4_sub(cam_W, v0_W)));
    tri->view1_T    = vec4_normalize(mat_mul_vec(tbn1_W_T, vec4_sub(cam_W, v1_W)));
    tri->view2_T    = vec4_normalize(mat_mul_vec(tbn2_W_T, vec4_sub(cam_W, v2_W)));
    
    // set light
    vec4_t light    = vec4_from_scalar(1.f);
    tri->light0_T   = vec4_normalize(mat_mul_vec(tbn0_W_T, light));
    tri->light1_T   = vec4_normalize(mat_mul_vec(tbn1_W_T, light));
    tri->light2_T   = vec4_normalize(mat_mul_vec(tbn2_W_T, light));

    // mvp transform
    tri->v0         = mat_mul_vec(PVM, v0);
    tri->v1         = mat_mul_vec(PVM, v1);
    tri->v2         = mat_mul_vec(PVM, v2);
}

uint32_t shader_process_fragment(triangle_t* tri, float w0, float w1, float w2)
{
    vec4_t one          = vec4_from_scalar(1.f);
    vec4_t light        = one;
    vec2_t t0           = tri->t0;
    vec2_t t1           = tri->t1;
    vec2_t t2           = tri->t2;

    float s             = f_min(t0.x * w0 + t1.x * w1 + t2.x * w2, 1.f);
    float t             = f_min(t0.y * w0 + t1.y * w1 + t2.y * w2, 1.f);

    vec4_t a            = vec4_from_bgra(sample(tri->albedo, s, t));
    vec4_t m_sample     = vec4_from_bgra(sample(tri->metallic, s, t));      // b = roughness, g = metallness
    float r             = m_sample.x;
    vec4_t m            = vec4_normalize(vec4_from_scalar(m_sample.y));
    vec4_t n_T          = vec4_from_bgra(sample(tri->normal, s, t));
    // vec_t o             = vec_from_bgra(sample(tri->occlusion, s, t));

    vec4_t view0_T      = vec4_scale(tri->view0_T, w0);
    vec4_t view1_T      = vec4_scale(tri->view1_T, w1);
    vec4_t view2_T      = vec4_scale(tri->view2_T, w2);
    vec4_t light0_T     = vec4_scale(tri->light0_T, w0);
    vec4_t light1_T     = vec4_scale(tri->light1_T, w1);
    vec4_t light2_T     = vec4_scale(tri->light2_T, w2);
    vec4_t v_T          = vec4_normalize(vec4_add(view0_T, vec4_add(view1_T, view2_T)));
    vec4_t l_T          = vec4_normalize(vec4_add(light0_T, vec4_add(light1_T, light2_T)));
    vec4_t h_T          = vec4_normalize(vec4_add(v_T, l_T));

    float n_dot_h       = f_max(vec4_dot(n_T, h_T), 0.f);
    float n_dot_v       = f_max(vec4_dot(n_T, v_T), 0.f);
    float n_dot_l       = f_max(vec4_dot(n_T, l_T), 0.f);

    // specular
    //      F * D * G
    // ---------------------
    // 4 * n_dot_l * n_dot_v

    vec4_t f            = fresnel(n_dot_h, a, m);
    float ndf           = normal_dist(n_dot_h, r);
    float g             = self_shadow(n_dot_v, n_dot_l, r);
    float f_scale       = ( ndf * g ) / ( 4 * n_dot_l * n_dot_v + 0.0001f );
    vec4_t specular     = vec4_scale(f, f_scale);

    // diffuse
    //   kd * c
    //  --------
    //     pi

    vec4_t kd           = vec4_sub(one, f);                                         // get fraction of light that gets refracted. fresnel acts as "ks"
    kd                  = vec4_hadamard(kd, vec4_sub(one, m));                      // kd for metallic surfaces should be 0;
    kd                  = vec4_hadamard(kd, vec4_from_scalar(1.f / F_PI));
    vec4_t diffuse      = vec4_hadamard(a, kd);

    vec4_t c            = vec4_add(diffuse, specular);
    c                   = vec4_hadamard(c, light);
    c                   = vec4_scale(c, n_dot_l);
    c                   = vec4_scale(c, 10.f);

    return vec4_to_bgra(c);
}
