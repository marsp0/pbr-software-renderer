#include "mesh.h"

#include <stdlib.h>
#include <string.h>

mesh_t* mesh_new(char* name, vec_t* vertices, vec_t* texcoords, vec_t* normals, 
                 int vertices_size, int texcoords_size, int normals_size,
                 int* vertex_indices, int* texcoord_indices, int* normal_indices, 
                 int indices_size)
{
    mesh_t* mesh = malloc(sizeof(mesh_t));
    
    strncpy(mesh->name, name, MESH_NAME_SIZE - 1);
    mesh->vertices = vertices;
    mesh->texcoords = texcoords;
    mesh->normals = normals;

    mesh->vertices_size = vertices_size;
    mesh->texcoords_size = texcoords_size;
    mesh->normals_size = normals_size;
    
    mesh->vertex_indices = vertex_indices;
    mesh->texcoord_indices = texcoord_indices;
    mesh->normal_indices = normal_indices;
    
    mesh->indices_size = indices_size;
    
    return mesh;
}

void mesh_free(mesh_t* mesh)
{
    free(mesh->vertices);
    free(mesh->texcoords);
    free(mesh->normals);
    free(mesh->vertex_indices);
    free(mesh->texcoord_indices);
    free(mesh->normal_indices);
    free(mesh);
}