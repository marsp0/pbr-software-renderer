#include "mesh.h"

#include <stdlib.h>
#include <string.h>

mesh* mesh_new(char* name, vec* vertices, vec* texcoords, vec* normals, 
               uint32_t vertices_size, uint32_t texcoords_size, uint32_t normals_size,
               int* vertex_indices, int* texcoord_indices, int* normal_indices, 
               uint32_t indices_size)
{
	mesh* result = malloc(sizeof(mesh));
	
	strncpy(result->name, name, MESH_NAME_SIZE);
	result->vertices = vertices;
	result->texcoords = texcoords;
	result->normals = normals;

	result->vertices_size = vertices_size;
	result->texcoords_size = texcoords_size;
	result->normals_size = normals_size;
	
	result->vertex_indices = vertex_indices;
	result->texcoord_indices = texcoord_indices;
	result->normal_indices = normal_indices;
	
	result->indices_size = indices_size;
	
	return result;
}

void mesh_free(mesh* mesh_data)
{
	free(mesh_data->vertices);
	free(mesh_data->texcoords);
	free(mesh_data->normals);
	free(mesh_data->vertex_indices);
	free(mesh_data->texcoord_indices);
	free(mesh_data->normal_indices);
	free(mesh_data);
}