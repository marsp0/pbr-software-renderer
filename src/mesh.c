#include "mesh.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

    mesh->triangles_size    = indices_size / 3U;
    uint32_t size_in_bytes  = sizeof(triangle_t) * mesh->triangles_size;
    mesh->triangles         = malloc(size_in_bytes);
    memset(mesh->triangles, 0, size_in_bytes);
    
    return mesh;
}

void mesh_free(mesh_t* mesh)
{
    free(mesh->vertices);
    free(mesh->texcoords);
    free(mesh->normals);
    free(mesh->indices);
    free(mesh->triangles);
    texture_free(mesh->albedo);
    texture_free(mesh->metallic);
    texture_free(mesh->normal);
    texture_free(mesh->occlusion);
    free(mesh);
}