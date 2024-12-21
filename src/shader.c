#include "shader.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "texture.h"

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

static camera_t* camera     = NULL;
static texture_t* albedo    = NULL;
static texture_t* metallic  = NULL;
static texture_t* normal    = NULL;
static mat_t  tbn;
static vec2_t t0;
static vec2_t t1;
static vec2_t t2;
static vec4_t view0_T;
static vec4_t view1_T;
static vec4_t view2_T;
static vec4_t light0_T;
static vec4_t light1_T;
static vec4_t light2_T;

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
    // float k_direct  = (r + 1) * (r + 1) / 8;
    float k_direct  = (r * r) / 2;
    // float k_ibl     = a * a / 2
    float ggx_1     = schlick_ggx(n_dot_v, k_direct);
    float ggx_2     = schlick_ggx(n_dot_l, k_direct);
    float result    = ggx_1 * ggx_2;

    return result;
}

// Fresnel factor describing ratio of reflected vs refracted light
static vec4_t fresnel(float h_dot_v, vec4_t a, vec4_t m)
{
    vec4_t one      = vec4_new(1.f, 1.f, 1.f);

    // Fresnel-Schlick approx
    vec4_t f0       = vec4_from_scalar(0.04f);
    f0              = vec4_mix(f0, a, m);
    vec4_t diff     = vec4_sub(one, f0);
    float exp       = (float)pow(f_clamp(1.f - h_dot_v, 0.f, 1.f), 5.f);
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

void shader_set_uniforms(camera_t* cam,
                         texture_t* a, 
                         texture_t* m, 
                         texture_t* n,
                         vec4_t v0, 
                         vec4_t v1, 
                         vec4_t v2,
                         vec2_t tex_coord0, 
                         vec2_t tex_coord1, 
                         vec2_t tex_coord2)
{
    camera          = cam;
    vec4_t cam_w    = camera->position;

    albedo          = a;
    metallic        = m;
    normal          = n;

    t0              = tex_coord0;
    t1              = tex_coord1;
    t2              = tex_coord2;
    
    // vec4_t n0       = vec4_normalize(vec4_cross(vec4_sub(v1, v0), vec4_sub(v2, v0)));
    
    
    // mat_t M         = mat_new_identity();
    // mat_t N         = mat_transpose(mat_inverse(M));
    
    // vec4_t v0_w     = mat_mul_vec(M, v0);
    // vec4_t v1_w     = mat_mul_vec(M, v1);
    // vec4_t v2_w     = mat_mul_vec(M, v2);

    // // compute tangent
    // vec4_t e1       = vec4_sub(v1, v0);
    // vec4_t e2       = vec4_sub(v2, v0);
    // vec2_t d_st1    = vec2_sub(t1, t0);
    // vec2_t d_st2    = vec2_sub(t2, t0);
    // float scale     = 1.f / (d_st1.x * d_st2.y - d_st2.x * d_st1.y);    // denom for inverse of uv mat
    // float tx        = (d_uv2.y * e1.x - d_uv1.y * e2.x) * scale;
    // float ty        = (d_uv2.y * e1.y - d_uv1.y * e2.y) * scale;
    // float tz        = (d_uv2.y * e1.z - d_uv1.y * e2.z) * scale;
    // vec4_t t_w      = vec4_normalize(mat_mul_vec(N, vec4_new(tx, ty, tz)));
    // vec4_t n0_w     = vec4_normalize(mat_mul_vec(N, n0));
    // vec4_t b_w      = vec4_normalize(mat_mul_vec(N, vec4_cross(n0_w, t_w)));
    // tbn             = mat_from_vec4(t_w, b_w, n0_w);

    // // get normal/tangent/bitangent in world space
    // vec4_t n0_w     = vec4_normalize(mat_mul_vec(N, n0));
    // vec4_t n1_w     = vec4_normalize(mat_mul_vec(N, n1));
    // vec4_t n2_w     = vec4_normalize(mat_mul_vec(N, n2));

    // vec4_t tan0_W   = mat_mul_vec(N, vec4_sub(tan, vec4_scale(n0, vec4_dot(n0, tan))));
    // vec4_t tan1_W   = mat_mul_vec(N, vec4_sub(tan, vec4_scale(n1, vec4_dot(n1, tan))));
    // vec4_t tan2_W   = mat_mul_vec(N, vec4_sub(tan, vec4_scale(n2, vec4_dot(n2, tan))));
    
    // vec4_t btan0_W  = vec4_normalize(vec4_cross(tan0_W, n0_w));
    // vec4_t btan1_W  = vec4_normalize(vec4_cross(tan1_W, n1_w));
    // vec4_t btan2_W  = vec4_normalize(vec4_cross(tan2_W, n2_w));
    
    // mat_t tbn0_W_T  = mat_transpose(mat_from_vec4(tan0_W, btan0_W, n0_w));
    // mat_t tbn1_W_T  = mat_transpose(mat_from_vec4(tan1_W, btan1_W, n1_w));
    // mat_t tbn2_W_T  = mat_transpose(mat_from_vec4(tan2_W, btan2_W, n2_w));

    // // set view directions
    // view0_T         = vec4_normalize(mat_mul_vec(tbn0_W_T, vec4_sub(cam_w, v0_w)));
    // view1_T         = vec4_normalize(mat_mul_vec(tbn1_W_T, vec4_sub(cam_w, v1_w)));
    // view2_T         = vec4_normalize(mat_mul_vec(tbn2_W_T, vec4_sub(cam_w, v2_w)));
    
    // // set light
    // vec4_t light    = vec4_from_scalar(-1.f);
    // light0_T        = vec4_normalize(mat_mul_vec(tbn0_W_T, light));
    // light1_T        = vec4_normalize(mat_mul_vec(tbn1_W_T, light));
    // light2_T        = vec4_normalize(mat_mul_vec(tbn2_W_T, light));
}

vec4_t shader_vertex(vec4_t v)
{
    mat_t M         = mat_new_identity();
    mat_t V         = camera_view_transform(camera);
    mat_t P         = camera_proj_transform(camera);
    mat_t VM        = mat_mul_mat(V, M);
    mat_t PVM       = mat_mul_mat(P, VM);
    
    return mat_mul_vec(PVM, v);
}

uint32_t shader_fragment(float w0, float w1, float w2)
{
    vec4_t one          = vec4_from_scalar(1.f);
    vec4_t light        = one;

    float s             = f_min(t0.x * w0 + t1.x * w1 + t2.x * w2, 1.f);
    float t             = f_min(t0.y * w0 + t1.y * w1 + t2.y * w2, 1.f);

    vec4_t a            = vec4_scale(vec4_from_bgra(sample(albedo, s, t)), 2.2f); // TODO: WHAT IS THIS 2.2f
    vec4_t m_sample     = vec4_from_bgra(sample(metallic, s, t));      // y/g = roughness, r/z = metallness
    float r             = m_sample.y;
    vec4_t m            = vec4_normalize(vec4_from_scalar(m_sample.z));
    vec4_t n_T          = vec4_normalize(vec4_from_bgra(sample(normal, s, t)));
    // vec_t o             = vec_from_bgra(sample(tri->occlusion, s, t));

    vec4_t view0_scaled_T      = vec4_scale(view0_T, w0);
    vec4_t view1_scaled_T      = vec4_scale(view1_T, w1);
    vec4_t view2_scaled_T      = vec4_scale(view2_T, w2);
    vec4_t light0_scaled_T     = vec4_scale(light0_T, w0);
    vec4_t light1_scaled_T     = vec4_scale(light1_T, w1);
    vec4_t light2_scaled_T     = vec4_scale(light2_T, w2);
    vec4_t v_T          = vec4_normalize(vec4_add(view0_scaled_T, vec4_add(view1_scaled_T, view2_scaled_T)));
    vec4_t l_T          = vec4_normalize(vec4_add(light0_scaled_T, vec4_add(light1_scaled_T, light2_scaled_T)));
    vec4_t h_T          = vec4_normalize(vec4_add(v_T, l_T));

    float n_dot_h       = f_max(vec4_dot(n_T, h_T), 0.f);
    float n_dot_v       = f_max(vec4_dot(n_T, v_T), 0.f);
    float n_dot_l       = f_max(vec4_dot(n_T, l_T), 0.f);
    float h_dot_v       = f_max(vec4_dot(h_T, v_T), 0.f);

    // specular
    //      F * D * G
    // ---------------------
    // 4 * n_dot_l * n_dot_v

    float d             = normal_dist(n_dot_h, r);
    float g             = self_shadow(n_dot_v, n_dot_l, r);
    vec4_t f            = fresnel(h_dot_v, a, m);
    float dg            = ( d * g ) / ( 4 * n_dot_l * n_dot_v + 0.000001f );
    vec4_t specular     = vec4_scale(f, dg);

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

    return vec4_to_bgra(c);
}