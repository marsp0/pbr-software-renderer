#include "test_scene.h"

#include "../scene.h"
#include "test_utils.h"

static void test_water_bottle()
{
	const char* file_path = "./assets/WaterBottle.glb";

	scene_t* scene = scene_new(file_path);

	ASSERT_TRUE(scene != NULL);
	ASSERT_TRUE(scene->mesh != NULL);
	ASSERT_TRUE(scene->camera != NULL);

	ASSERT_EQUAL(scene->mesh->indices_size, 13530);
	ASSERT_EQUAL(scene->mesh->vertices_size, 2549);
	ASSERT_EQUAL(scene->mesh->normals_size, 2549);
	ASSERT_EQUAL(scene->mesh->texcoords_size, 2549);

	ASSERT_EQUAL(scene->mesh->albedo->width, 2048);
	ASSERT_EQUAL(scene->mesh->albedo->height, 2048);
	ASSERT_EQUAL(scene->mesh->albedo->stride, 3);

	ASSERT_EQUAL(scene->mesh->metallic->width, 2048);
	ASSERT_EQUAL(scene->mesh->metallic->height, 2048);
	ASSERT_EQUAL(scene->mesh->metallic->stride, 3);

	ASSERT_EQUAL(scene->mesh->normal->width, 2048);
	ASSERT_EQUAL(scene->mesh->normal->height, 2048);
	ASSERT_EQUAL(scene->mesh->normal->stride, 3);

	ASSERT_EQUAL(scene->mesh->occlusion->width, 2048);
	ASSERT_EQUAL(scene->mesh->occlusion->height, 2048);
	ASSERT_EQUAL(scene->mesh->occlusion->stride, 3);

	scene_free(scene);
}

void test_scene()
{
	TEST_CASE(test_water_bottle);
}