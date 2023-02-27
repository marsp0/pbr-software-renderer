#include "scene_validator.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "../constants.h"

static void assert_container(const json_node_t* node, uint32_t size, const char* name)
{
	if (!node)
	{
		printf("invalid node: %s\n", name);
		assert(false);
	}
	if (node->size < size)
	{
		printf("invalid node size: %s\n", name);
		assert(false);
	}
}

static uint32_t assert_index(const json_node_t* node, uint32_t count, const char* name)
{
	if (!node)
	{
		printf("invalid index node: %s\n", name);
		assert(false);
	}

	return node->uinteger > count ? node->uinteger : count;
}

static void validate_asset(json_t* json)
{
	const json_node_t* asset = json_find_node(json, 1, JSON_ASSET);
	assert_container(asset, 1, JSON_ASSET);

	const json_node_t* version = json_find_child(asset, JSON_VERSION);
	assert(version);
	assert(strncmp(version->string, "2.0", (uint64_t)version->string_size) == 0);
}

static void validate_scene(json_t* json)
{
	/*scene exists*/
	const json_node_t* scenes = json_find_node(json, 1, JSON_SCENES);
	assert_container(scenes, 1, JSON_SCENES);
	const json_node_t* scene = json_find_index(scenes, 0);

	/*scene has at least 1 mesh*/
	const json_node_t* scene_nodes = json_find_child(scene, JSON_NODES);
	assert_container(scene_nodes, 1, "scene->nodes");
}

static void validate_nodes(json_t* json)
{
	const json_node_t* nodes = json_find_node(json, 1, JSON_NODES);
	assert_container(nodes, 1, JSON_NODES);

	const json_node_t* current = json_find_index(nodes, 0);
	uint32_t mesh_count = 0;
	
	while(current)
	{
		assert_container(current, 2, "nodes->node");

		const json_node_t* mesh = json_find_child(current, JSON_MESH);
		mesh_count = assert_index(mesh, mesh_count, JSON_MESH);

		const json_node_t* name = json_find_child(current, JSON_NAME);
		assert(name);

		current = current->next;
	}

	/* validate number of mesh nodes */
	const json_node_t* meshes = json_find_node(json, 1, JSON_MESHES);
	assert_container(meshes, mesh_count + 1, JSON_MESHES);
}

static void validate_meshes(json_t* json)
{
	const json_node_t* meshes = json_find_node(json, 1, JSON_MESHES);
	const json_node_t* mesh = json_find_index(meshes, 0);

	uint32_t material_count = 0;
	uint32_t accessors_count = 0;
	
	while(mesh)
	{
		assert_container(mesh, 1, "mesh");

		/* TODO: multiple primitives per mesh */
		const json_node_t* primitives = json_find_child(mesh, JSON_PRIMITIVES);
		assert_container(primitives, 1, JSON_PRIMITIVES);

		const json_node_t* primitive = json_find_index(primitives, 0);
		while(primitive)
		{
			/* validate primitive */
			assert_container(primitive, 3, "primitive");

			const json_node_t* indices = json_find_child(primitive, JSON_INDICES);
			accessors_count = assert_index(indices, accessors_count, JSON_INDICES);

			const json_node_t* material = json_find_child(primitive, JSON_MATERIAL);
			material_count = assert_index(material, material_count, JSON_MATERIAL);

			/* validate attributes */
			const json_node_t* attributes = json_find_child(primitive, JSON_ATTRIBUTES);
			assert_container(attributes, 3, JSON_ATTRIBUTES);

			const json_node_t* position = json_find_child(attributes, JSON_POSITION);
			accessors_count = assert_index(position, accessors_count, JSON_POSITION);

			const json_node_t* texture = json_find_child(attributes, JSON_TEXCOORD_0);
			accessors_count = assert_index(texture, accessors_count, JSON_TEXCOORD_0);

			const json_node_t* normal = json_find_child(attributes, JSON_NORMAL);
			accessors_count = assert_index(normal, accessors_count, JSON_NORMAL);

			primitive = primitive->next;
		}

		mesh = mesh->next;
	}

	const json_node_t* accessors = json_find_node(json, 1, JSON_ACCESSORS);
	const json_node_t* materials = json_find_node(json, 1, JSON_MATERIALS);
	assert_container(accessors, accessors_count + 1, JSON_ACCESSORS);
	assert_container(materials, material_count + 1, JSON_MATERIALS);
}

static void validate_materials(json_t* json)
{
	const json_node_t* materials = json_find_node(json, 1, JSON_MATERIALS);
	const json_node_t* current = materials->child;

	uint32_t texture_count = 0;
	
	while(current)
	{
		const json_node_t* pbr_node = json_find_child(current, JSON_PBR);
		assert_container(pbr_node, 2, JSON_PBR);
		
		const json_node_t* base = json_find_child(pbr_node, JSON_ALBEDO_TEX);
		assert_container(base, 1, JSON_ALBEDO_TEX);
		const json_node_t* index = json_find_child(base, JSON_INDEX);
		texture_count = assert_index(index, texture_count, JSON_INDEX);
		
		const json_node_t* metallic = json_find_child(pbr_node, JSON_MR_TEX);
		assert_container(metallic, 1, JSON_MR_TEX);
		index = json_find_child(metallic, JSON_INDEX);
		texture_count = assert_index(index, texture_count, JSON_INDEX);

		const json_node_t* normal = json_find_child(current, JSON_NORMAL_TEX);
		assert_container(normal, 1, JSON_NORMAL_TEX);
		index = json_find_child(normal, JSON_INDEX);
		texture_count = assert_index(index, texture_count, JSON_INDEX);

		const json_node_t* occlusion = json_find_child(current, JSON_OCCLUSION_TEX);
		assert_container(occlusion, 1, JSON_OCCLUSION_TEX);
		index = json_find_child(occlusion, JSON_INDEX);
		texture_count = assert_index(index, texture_count, JSON_INDEX);

		current = current->next;
	}

	const json_node_t* textures = json_find_node(json, 1, JSON_TEXTURES);
	assert_container(textures, texture_count + 1, JSON_TEXTURES);
}

static void validate_accessors(json_t* json)
{
	const json_node_t* accessors = json_find_node(json, 1, JSON_ACCESSORS);
	
	const json_node_t* accessor = json_find_index(accessors, 0);

	uint32_t view_count = 0;

	while (accessor)
	{
		assert_container(accessor, 4, "accessor");
		const json_node_t* view = json_find_child(accessor, JSON_BUFFER_VIEW);
		view_count = assert_index(view, view_count, JSON_BUFFER_VIEW);

		accessor = accessor->next;
	}

	const json_node_t* views = json_find_node(json, 1, JSON_BUFFER_VIEWS);
	assert_container(views, view_count + 1, JSON_BUFFER_VIEWS);
}

static void validate_textures(json_t* json)
{
	const json_node_t* textures = json_find_node(json, 1, JSON_TEXTURES);

	const json_node_t* texture = json_find_index(textures, 0);
	uint32_t image_count = 0;

	while (texture)
	{
		assert_container(texture, 1, "texture");

		const json_node_t* source = json_find_child(texture, JSON_SOURCE);
		image_count = assert_index(source, image_count, JSON_SOURCE);

		texture = texture->next;
	}
	const json_node_t* images = json_find_node(json, 1, JSON_IMAGES);
	assert_container(images, image_count + 1, JSON_IMAGES);
}

static void validate_images(json_t* json)
{
	const json_node_t* images = json_find_node(json, 1, JSON_IMAGES);

	const json_node_t* image = json_find_index(images, 0);
	uint32_t view_count = 0;

	while (image)
	{
		const json_node_t* mime = json_find_child(image, JSON_MIME_TYPE);
		assert(strncmp(mime->string, "image/png", (uint64_t)mime->string_size) == 0);

		const json_node_t* view = json_find_child(image, JSON_BUFFER_VIEW);
		view_count = assert_index(view, view_count, JSON_BUFFER_VIEW);

		image = image->next;
	}

	const json_node_t* views = json_find_node(json, 1, JSON_BUFFER_VIEWS);
	assert_container(views, view_count + 1, JSON_BUFFER_VIEWS);
}

static void validate_buffer_views(json_t* json)
{
	const json_node_t* views = json_find_node(json, 1, JSON_BUFFER_VIEWS);
	const json_node_t* view = json_find_index(views, 0);

	uint32_t buffer_count = 0;

	while (view)
	{
		assert_container(view, 2, "bufferView");

		const json_node_t* buf_index = json_find_child(view, JSON_BUFFER);
		buffer_count = assert_index(buf_index, buffer_count, JSON_BUFFER);

		view = view->next;
	}

	const json_node_t* buffers = json_find_node(json, 1, JSON_BUFFERS);
	assert_container(buffers, buffer_count + 1, JSON_BUFFERS);
}

void validate_scene(json_t* json)
{
	validate_asset(json);
	validate_scene(json);
	validate_nodes(json);
	validate_meshes(json);
	validate_materials(json);
	validate_accessors(json);
	validate_textures(json);
	validate_images(json);
	validate_buffer_views(json);

	// TODO: add type validation  (VEC/SCALAR etc) to accessor validation
}