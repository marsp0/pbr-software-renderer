#pragma once

#include <stdint.h>

#include "math.h"
#include "constants.h"

typedef struct
{
    char        name[MESH_NAME_SIZE];
    vec*        vertices;
    vec*        texcoords;
    vec*        normals;
    uint32_t    vertices_size;
    uint32_t    texcoords_size;
    uint32_t    normals_size;
    int*        vertex_indices;
    int*        texcoord_indices;
    int*        normal_indices;
    uint32_t    indices_size;

} mesh;

mesh* mesh_new(char* name, vec* vertices, vec* texcoords, vec* normals, 
               uint32_t vertices_size, uint32_t texcoords_size, uint32_t normals_size,
               int* vertex_indices, int* texcoord_indices, int* normal_indices, 
               uint32_t indices_size);

void mesh_free(mesh* mesh);