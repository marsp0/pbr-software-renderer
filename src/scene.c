#include "scene.h"

#include <stdio.h>
#include <stdlib.h>

#include "parsers/glb.h"

/********************
 *  Notes
 *
 ********************/

/********************/
/*      defines     */
/********************/

/********************/
/* static variables */
/********************/

/********************/
/* static functions */
/********************/

/********************/
/* public functions */
/********************/

scene_t* scene_new(const char* file_path)
{
	scene_t* scene = parse_scene(file_path);
	return scene;
}

void scene_reset(scene_t* scene)
{
	// reset camera position
	// reset some other stuff
	printf("resetting scene %p", (void*)scene);
}

void scene_free(scene_t* scene)
{
	mesh_free(scene->mesh);
	free(scene);
}