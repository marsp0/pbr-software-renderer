#pragma once

#include <stdint.h>

#include "math.h"
#include "constants.h"
#include "texture.h"

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
                 texture_t* occlusion);

void mesh_free(mesh_t* mesh);