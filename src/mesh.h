#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "math.h"
#include "texture.h"

#define MESH_NAME_SIZE 256
#define MAX_MESHES 20

typedef struct
{
    bool        process;

    vec4_t      v0;
    vec4_t      v1;
    vec4_t      v2;

    vec2_t      t0;
    vec2_t      t1;
    vec2_t      t2;
    
    vec4_t      n0;
    vec4_t      n1;
    vec4_t      n2;

    vec4_t      view0_T;
    vec4_t      view1_T;
    vec4_t      view2_T;
    
    vec4_t      light0_T;
    vec4_t      light1_T;
    vec4_t      light2_T;

    texture_t*  albedo;
    texture_t*  metallic;
    texture_t*  normal;
    texture_t*  occlusion;

} triangle_t;

typedef struct
{
    char        name[MESH_NAME_SIZE];
    vec4_t*     vertices;
    vec2_t*     texcoords;
    vec4_t*     normals;
    uint32_t*   indices;
    triangle_t* triangles;

    uint32_t    vertices_size;
    uint32_t    texcoords_size;
    uint32_t    normals_size;
    uint32_t    indices_size;
    uint32_t    triangles_size;

    texture_t*  albedo;
    texture_t*  metallic;
    texture_t*  normal;
    texture_t*  occlusion;
    sphere_t    bounding_sphere;
} mesh_t;

mesh_t* mesh_new(char*      name,
                 vec4_t*    vertices,
                 vec2_t*    texcoords,
                 vec4_t*    normals,
                 uint32_t*  indices,
                 uint32_t   vertices_size,
                 uint32_t   texcoords_size,
                 uint32_t   normals_size,
                 uint32_t   indices_size,
                 texture_t* albedo,
                 texture_t* metallic,
                 texture_t* normal,
                 texture_t* occlusion,
                 sphere_t   bsphere);

void mesh_free(mesh_t* mesh);