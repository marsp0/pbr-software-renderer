#pragma once

#include <stdint.h>

#include "math.h"
#include "constants.h"

typedef struct
{
    char    name[MESH_NAME_SIZE];
    vec_t*  vertices;
    vec_t*  texcoords;
    vec_t*  normals;
    int     vertices_size;
    int     texcoords_size;
    int     normals_size;
    int*    vertex_indices;
    int*    texcoord_indices;
    int*    normal_indices;
    int     indices_size;

} mesh_t;

mesh_t* mesh_new(char* name, 
                 vec_t* vertices, 
                 vec_t* texcoords, 
                 vec_t* normals, 
                 int vertices_size, 
                 int texcoords_size, 
                 int normals_size,
                 int* vertex_indices, 
                 int* texcoord_indices, 
                 int* normal_indices, 
                 int indices_size);

void mesh_free(mesh_t* mesh);