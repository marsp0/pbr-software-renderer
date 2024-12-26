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

void scene_update(scene_t* scene, input_t input, float dt)
{
    camera_update(scene->camera, input, dt);
}

void scene_free(scene_t* scene)
{
    mesh_free(scene->mesh);
    camera_free(scene->camera);
    free(scene);
}