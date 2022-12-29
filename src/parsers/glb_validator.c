#include "glb_validator.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

static void assert_container(const json_node_t* node)
{
	assert(node);
	assert(node->size > 0);
}

static void validate_asset(json_t* json)
{
	const json_node_t* asset = json_find_node(json, 1, "asset");
	assert_container(asset);

	const json_node_t* version = json_find_child(asset, "version");
	assert(version);
	assert(strncmp(version->string, "2.0", (uint64_t)version->string_size) == 0);
}

static void validate_scene(json_t* json)
{
	/*scene exists*/
	const json_node_t* scenes = json_find_node(json, 1, "scenes");
	assert_container(scenes);
	const json_node_t* scene = json_find_array_element(scenes, 0);

	/*scene has at least 1 mesh*/
	const json_node_t* scene_nodes = json_find_child(scene, "nodes");
	assert_container(scene_nodes);
}

static void validate_nodes(json_t* json)
{
	const json_node_t* nodes = json_find_node(json, 1, "nodes");
	assert_container(nodes);

	const json_node_t* current = json_find_array_element(nodes, 0);
	while(current)
	{
		assert_container(current);
		const json_node_t* mesh = json_find_child(current, "mesh");
		assert(mesh);
		const json_node_t* name = json_find_child(current, "name");
		assert(name);
		current = current->next;
	}
}

static void validate_materials(json_t* json)
{
	const json_node_t* materials = json_find_node(json, 1, "materials");
	assert_container(materials);

	const json_node_t* current = materials->child;
	while(current)
	{
		const json_node_t* pbr_node = json_find_child(current, "pbrMetallicRoughness");
		assert_container(pbr_node);
		
		const json_node_t* base = json_find_child(pbr_node, "baseColorTexture");
		assert_container(base);
		
		const json_node_t* metallic = json_find_child(pbr_node, "metallicRoughnessTexture");
		assert_container(metallic);

		const json_node_t* normal = json_find_child(current, "normalTexture");
		assert(normal);

		const json_node_t* occlusion = json_find_child(current, "occlusionTexture");
		assert(occlusion);

		current = current->next;
	}
}

static void validate_meshes(json_t* json)
{
	const json_node_t* meshes = json_find_node(json, 1, "meshes");
	assert_container(meshes);

	const json_node_t* mesh = json_find_array_element(meshes, 0);
	while(mesh)
	{
		assert_container(mesh);

		const json_node_t* primitives = json_find_child(mesh, "primitives");
		assert_container(primitives);

		const json_node_t* primitive = json_find_array_element(primitives, 0);
		while(primitive)
		{
			assert_container(primitive);

			const json_node_t* attributes = json_find_child(primitive, "attributes");
			assert_container(attributes);

			const json_node_t* position = json_find_child(attributes, "POSITION");
			assert(position);

			const json_node_t* texture = json_find_child(attributes, "TEXCOORD_0");
			assert(texture);

			const json_node_t* material = json_find_child(primitive, "material");
			assert(material);

			primitive = primitive->next;
		}

		mesh = mesh->next;
	}
}

void validate_glb(json_t* json)
{
	validate_asset(json);
	validate_scene(json);
	validate_nodes(json);
	validate_materials(json);
	validate_meshes(json);
}