#pragma once

#include "mesh.h"
#include "camera.h"

typedef struct
{
	mesh_t* 	mesh;
	camera_t* 	camera;
	// vec_t 	dir_light;
	// vec_t 	point_light;
} scene_t;

scene_t* 	scene_new(const char* file_path);
void 		scene_reset(scene_t* scene);
void 		scene_free(scene_t* scene);