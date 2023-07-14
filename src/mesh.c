#include "mesh.h"

#include <stdlib.h>
#include <string.h>

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
                 sphere_t   bsphere)
{
    mesh_t* mesh = malloc(sizeof(mesh_t));
    
    strncpy(mesh->name, name, MESH_NAME_SIZE - 1);
    mesh->vertices          = vertices;
    mesh->texcoords         = texcoords;
    mesh->normals           = normals;

    mesh->vertices_size     = vertices_size;
    mesh->texcoords_size    = texcoords_size;
    mesh->normals_size      = normals_size;
    
    mesh->indices           = indices;
    mesh->indices_size      = indices_size;

    mesh->albedo            = albedo;
    mesh->metallic          = metallic;
    mesh->normal            = normal;
    mesh->occlusion         = occlusion;
    mesh->bounding_sphere   = bsphere;
    
    return mesh;
}

void mesh_free(mesh_t* mesh)
{
    free(mesh->vertices);
    free(mesh->texcoords);
    free(mesh->normals);
    free(mesh->indices);
    free(mesh->albedo);
    free(mesh->metallic);
    free(mesh->normal);
    free(mesh->occlusion);
    free(mesh);
}