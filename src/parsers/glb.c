#include "glb.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "png.h"
#include "json.h"
#include "scene_validator.h"
#include "json_scene_constants.h"

// GLTF 2 specification - https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html

/********************/
/* static variables */
/********************/

typedef struct
{
    uint32_t size;
    uint32_t type; // 0x4E4F534A or 0x004E4942
    const unsigned char* data;
} chunk_t;

typedef struct
{
    // const unsigned char* 
    int32_t data;
    uint32_t start;
    uint32_t size;
} data_t;

static uint32_t cursor = 0;

/********************/
/* static functions */
/********************/

static uint32_t parse_int(const unsigned char* buffer, uint32_t n)
{
    assert(n <= 4);

    uint32_t result = 0;
    for (uint32_t i = cursor; i < cursor + n; i++)
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

    // assert that chunk is either bin or json
    assert(chunk.type == 0x4E4F534A || chunk.type == 0x004E4942);

    return chunk;
}

static void parse_header(const unsigned char* buffer)
{
    uint32_t magic = parse_int(buffer, 4);
    uint32_t version = parse_int(buffer, 4);
    cursor += 4;

    assert(magic == 0x46546C67);
    assert(version == 2);
}

static void parse_mesh_data(const json_t* json, const json_node_t* node, const char* attribute)
{
    // node has an index from the accessors array
    // accessors array contains indices from the buffer views array
    // buffers array holds data used to access the binary chunks

    const json_node_t* accessors = json_find_node(json, 1, JSON_ACCESSORS);
    const json_node_t* views     = json_find_node(json, 1, JSON_BUFFER_VIEWS);

    const json_node_t* index     = json_find_child(node, attribute);
    const json_node_t* accessor  = json_find_index(accessors, index->uinteger);
    
    index                        = json_find_child(accessor, JSON_BUFFER_VIEW);
    const json_node_t* view      = json_find_index(views, index->uinteger);
    const json_node_t* buffer    = json_find_child(view, JSON_BUFFER);
    const json_node_t* offset    = json_find_child(view, JSON_BYTE_OFFSET);
    const json_node_t* length    = json_find_child(view, JSON_BYTE_LEN);
    printf("%s: %d/%d/%d\n", attribute, buffer->integer, offset ? offset->integer : 0, length->integer);

    return;
}

static void parse_material_texture(const json_t* json, const json_node_t* node)
{
    // Material node has contains indices from textures array
    // textures array contain indices from images array
    // images array contains indices from buffer views array
    // buffer views array contains indices (offset + len too) from buffers array
    // buffers array holds data used to access the binary chunks

    const json_node_t* textures  = json_find_node(json, 1, JSON_TEXTURES);
    const json_node_t* images    = json_find_node(json, 1, JSON_IMAGES);
    const json_node_t* views     = json_find_node(json, 1, JSON_BUFFER_VIEWS);

    const json_node_t* index     = json_find_child(node, JSON_INDEX);
    const json_node_t* texture   = json_find_index(textures, index->uinteger);

    index                        = json_find_child(texture, JSON_SOURCE);
    const json_node_t* image     = json_find_index(images, index->uinteger);

    index                        = json_find_child(image, JSON_BUFFER_VIEW);
    const json_node_t* view      = json_find_index(views, index->uinteger);

    const json_node_t* buffer    = json_find_child(view, JSON_BUFFER);
    const json_node_t* offset    = json_find_child(view, JSON_BYTE_OFFSET);
    const json_node_t* length    = json_find_child(view, JSON_BYTE_LEN);
    printf("View: %d/%d/%d\n", buffer->integer, offset ? offset->integer : 0, length->integer);

    return;
}

static void parse_meshes(const json_t* json)
{
    const json_node_t* scenes       = json_find_node(json, 1, JSON_SCENES);
    const json_node_t* nodes        = json_find_node(json, 1, JSON_NODES);
    const json_node_t* meshes       = json_find_node(json, 1, JSON_MESHES);
    const json_node_t* materials    = json_find_node(json, 1, JSON_MATERIALS);
    const json_node_t* scene        = json_find_index(scenes, 0);
    const json_node_t* scene_nodes  = json_find_child(scene, JSON_NODES);
    const json_node_t* node_index   = json_find_index(scene_nodes, 0);
    
    while (node_index)
    {
        // get mesh
        const json_node_t* node         = json_find_index(nodes, node_index->uinteger);
        const json_node_t* index        = json_find_child(node, JSON_MESH);
        const json_node_t* mesh         = json_find_index(meshes, index->uinteger);
        const json_node_t* primitives   = json_find_child(mesh, JSON_PRIMITIVES);
        
        // parse vertex data
        const json_node_t* primitive    = json_find_index(primitives, 0);                     // TODO: allow multiple primitives
        const json_node_t* attributes   = json_find_child(primitive, JSON_ATTRIBUTES);
        
        parse_mesh_data(json, primitive, JSON_INDICES);
        parse_mesh_data(json, attributes, JSON_POSITION);
        parse_mesh_data(json, attributes, JSON_TEXCOORD_0);
        parse_mesh_data(json, attributes, JSON_NORMAL);

        // parse material data
        index                           = json_find_child(primitive, JSON_MATERIAL);
        const json_node_t* material     = json_find_index(materials, index->uinteger);
        const json_node_t* pbr          = json_find_child(material, JSON_PBR);
        const json_node_t* albedo       = json_find_child(pbr, JSON_ALBEDO_TEX);
        const json_node_t* metallic     = json_find_child(pbr, JSON_MR_TEX);
        const json_node_t* normal       = json_find_child(material, JSON_NORMAL_TEX);
        const json_node_t* occlusion    = json_find_child(material, JSON_OCCLUSION_TEX);

        parse_material_texture(json, albedo);
        parse_material_texture(json, metallic);
        parse_material_texture(json, normal);
        parse_material_texture(json, occlusion);

        node_index = node_index->next;
    }

}

/********************/
/* public functions */
/********************/

mesh_t* parse_scene(const char* file_name)
{

    // open file
    FILE* file = fopen(file_name, "r");
    assert(file != NULL);

    // get file size
    fseek(file, 0, SEEK_END);
    uint32_t file_size = (uint32_t)ftell(file);
    fseek(file, 0, SEEK_SET);

    // copy file contents into buffer
    unsigned char* buffer = malloc(file_size);
    fread(buffer, sizeof(unsigned char), file_size, file);

    // close file as its no longer needed
    fclose(file);

    printf("Allocating %.02fMB for %s\n", file_size / 1024.0 / 1024.0, file_name);

    cursor = 0;

    parse_header(buffer);

    // process json
    chunk_t json_chunk = parse_chunk(buffer);
    json_t* json = json_new(json_chunk.data, json_chunk.size);
    validate_glb_scene(json);

    parse_meshes(json);

    chunk_t binary_chunk = parse_chunk(buffer);

    parse_png(binary_chunk.data, 2165850);

    mesh_t* result = NULL;

    json_free(json);
    free(buffer);

    return result;
}