#include "glb.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

#include "../constants.h"
#include "png.h"
#include "json.h"

/*
 * GLTF 2 specification - https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html
 */

static int cursor = 0;

typedef struct
{
    uint32_t magic; /* 0x46546C67 */
    uint32_t version;
    uint32_t size;
} header_t;

typedef struct
{
    uint32_t size;
    uint32_t type; /* 0x4E4F534A or 0x004E4942 */
    const unsigned char* data;
} chunk_t;

static uint32_t parse_int(const unsigned char* buffer, int n)
{
    assert(n <= 4);

    uint32_t result = 0;
    for (int i = cursor; i < cursor + n; i++)
    {
        result += buffer[i] << ((i - cursor) * 8);
    }
    cursor += n;
    return result;
}

static chunk_t parse_chunk(const unsigned char* buffer)
{
    chunk_t chunk;
    chunk.size = parse_int(buffer, 4);
    chunk.type = parse_int(buffer, 4);
    chunk.data = &buffer[cursor];

    cursor += chunk.size;

    /* assert that chunk is either bin or json */
    assert(chunk.type == 0x4E4F534A || chunk.type == 0x004E4942);

    return chunk;
}

static header_t parse_header(const unsigned char* buffer)
{
    header_t header;
    header.magic = parse_int(buffer, 4);
    header.version = parse_int(buffer, 4);
    header.size = parse_int(buffer, 4);

    assert(header.version == 2);
    assert(header.magic == 0x46546C67);

    return header;
}

static mesh_t* parse_mesh(json_t* json, chunk_t binary, int32_t index)
{
    /*
     * 1. parse geometry
     * 2. parse materials
     */
    json_node_t* nodes = json_find_node(json, 1, "nodes");
    assert(nodes);
    json_node_t* node = json_find_array_element(nodes, index);
    assert(node);
    json_node_t* mesh_index = json_find_child(node, "mesh");
    assert(mesh_index);
    json_node_t* mesh_name = json_find_child(node, "name");
    assert(mesh_name);
    json_node_t* meshes = json_find_node(json, 1, "meshes");
    assert(meshes);
    json_node_t* mesh = json_find_array_element(meshes, mesh_index->integer);
    assert(mesh);
    json_node_t* primitives = json_find_child(mesh, "primitives");
    assert(primitives);
    json_node_t* primitive = json_find_array_element(primitives, 0);
    assert(primitive);
    json_node_t* attributes = json_find_child(primitive, "attributes");
    assert(attributes);
    json_node_t* pos_index = json_find_child(attributes, "POSITION");
    assert(pos_index);
    json_node_t* tex_index = json_find_child(attributes, "TEXCOORD_0");
    assert(tex_index);
    json_node_t* normal_index = json_find_child(attributes, "NORMAL");
    assert(normal_index);
    json_node_t* tan_index = json_find_child(attributes, "TANGENT");
    assert(tan_index);
}

mesh_t* parse_scene(const char* file_name)
{

    /* open file */
    FILE* file = fopen(file_name, "r");
    assert(file != NULL);

    /* get file size */
    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    /* copy file contents into buffer */
    unsigned char* buffer = malloc(file_size);
    fread(buffer, sizeof(unsigned char), file_size, file);

    /* close file as its no longer needed */
    fclose(file);

    printf("Allocating %.02fMB for %s\n", file_size / 1024.0 / 1024.0, file_name);

    cursor = 0;
    header_t header = parse_header(buffer);
    chunk_t json_chunk = parse_chunk(buffer);
    chunk_t binary_chunk = parse_chunk(buffer);
    json_t* json = json_new(json_chunk.data, json_chunk.size);

    json_node_t* scenes = json_find_node(json, 1, "scenes");
    assert(scenes);
    json_node_t* scene = scenes->child;
    assert(scene);
    json_node_t* scene_nodes = scene->child;
    assert(scene_nodes);
    json_node_t* node_index = scene_nodes->child;
    assert(node_index);

    mesh_t* result = NULL;

    while(node_index)
    {
        result = parse_mesh(json, binary_chunk, node_index->integer);
        node_index = node_index->next;
    }

    /* free the buffer */
    free(json);
    free(buffer);

    return result;
}