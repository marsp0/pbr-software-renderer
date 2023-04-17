#include "camera.h"

#include <stdlib.h>

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

camera_t* camera_new(vec_t position)
{
	camera_t* camera = malloc(sizeof(camera_t));
	camera->position = position;

	return camera;
}

void camera_free(camera_t* camera)
{
	free(camera);
}