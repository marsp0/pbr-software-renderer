#pragma once

#include <stdint.h>

#include "math.h"
#include "texture.h"

#define MESH_NAME_SIZE 256
#define MAX_MESHES 20

typedef struct
{
    char        name[MESH_NAME_SIZE];
    vec_t*      vertices;
    vec_t*      texcoords;
    vec_t*      normals;
    uint32_t*   indices;
    uint32_t    vertices_size;
    uint32_t    texcoords_size;
    uint32_t    normals_size;
    uint32_t    indices_size;
    texture_t*  albedo;
    texture_t*  metallic;
    texture_t*  normal;
    texture_t*  occlusion;
    sphere_t    bounding_sphere;
} mesh_t;

mesh_t* mesh_new(char*      name,
                 vec_t*     vertices,
                 vec_t*     texcoords,
                 vec_t*     normals,
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