#include "test_scene.h"

#include "../scene.h"
#include "test_utils.h"

static void test_water_bottle()
{
	const char* file_path = "./assets/WaterBottle.glb";

	scene_t* scene = scene_new(file_path);

	ASSERT_POINTER_EXISTS(scene);
	ASSERT_POINTER_EXISTS(scene->mesh);
	ASSERT_POINTER_EXISTS(scene->camera);

	ASSERT_UINT(scene->mesh->indices_size, 13530);
	ASSERT_UINT(scene->mesh->vertices_size, 2549);
	ASSERT_UINT(scene->mesh->normals_size, 2549);
	ASSERT_UINT(scene->mesh->texcoords_size, 2549);

	ASSERT_UINT(scene->mesh->albedo->width, 2048);
	ASSERT_UINT(scene->mesh->albedo->height, 2048);
	ASSERT_UINT(scene->mesh->albedo->stride, 3);

	ASSERT_UINT(scene->mesh->metallic->width, 2048);
	ASSERT_UINT(scene->mesh->metallic->height, 2048);
	ASSERT_UINT(scene->mesh->metallic->stride, 3);

	ASSERT_UINT(scene->mesh->normal->width, 2048);
	ASSERT_UINT(scene->mesh->normal->height, 2048);
	ASSERT_UINT(scene->mesh->normal->stride, 3);

	ASSERT_UINT(scene->mesh->occlusion->width, 2048);
	ASSERT_UINT(scene->mesh->occlusion->height, 2048);
	ASSERT_UINT(scene->mesh->occlusion->stride, 3);

	scene_free(scene);
}

void test_scene()
{
	test_water_bottle();
}