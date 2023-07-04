#include "glb.h"

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "png.h"
#include "json.h"
#include "../file.h"
#include "scene_validator.h"
#include "json_scene_constants.h"

// GLTF 2 specification - https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html

/********************/
/*      defines     */
/********************/

/********************/
/* static variables */
/********************/

typedef struct
{
    const unsigned char* data;
    uint32_t size;
    uint32_t type; // 0x4E4F534A or 0x004E4942
} chunk_t;

typedef struct
{
    const unsigned char* data;
    uint32_t size;
    uint32_t count;
} view_t;

/********************/
/* static functions */
/********************/

static uint32_t parse_bytes_lsb(file_t* file, const uint32_t n)
{
    assert(n <= 4);

    uint32_t result = 0;

    for (uint32_t i = file->cursor; i < file->cursor + n; i++)
    {
        result += file->data[i] << ((i - file->cursor) * 8);
    }

    file->cursor += n;
    return result;
}

static chunk_t parse_chunk(file_t* file)
{
    chunk_t chunk;
    chunk.size = parse_bytes_lsb(file, 4);
    chunk.type = parse_bytes_lsb(file, 4);
    chunk.data = &file->data[file->cursor];

    file->cursor += chunk.size;

    // assert that chunk is either bin or json
    assert(chunk.type == 0x4E4F534A || chunk.type == 0x004E4942);

    return chunk;
}

static void parse_header(file_t* file)
{
    uint32_t magic = parse_bytes_lsb(file, 4);
    uint32_t version = parse_bytes_lsb(file, 4);
    file->cursor += 4;

    assert(magic == 0x46546C67);
    assert(version == 2);
}

static uint32_t* create_indices_array(const view_t view)
{
    uint32_t j = 0;
    uint32_t* indices = malloc(sizeof(uint32_t) * view.count);

    for (uint32_t i = 0; i < view.size; i += 2)
    {
        indices[j] = (uint32_t)(view.data[i] + (view.data[i + 1] << 8));
        j++;
    }

    return indices;
}

static vec_t* create_vec_array(const view_t view)
{
    vec_t* result = malloc(sizeof(vec_t) * view.count);

    uint32_t j = 0;
    unsigned char float_arr[4] = { 0 };

    for (uint32_t i = 0; i < view.size; i += 12)
    {
        float_arr[0] = view.data[i + 0];
        float_arr[1] = view.data[i + 1];
        float_arr[2] = view.data[i + 2];
        float_arr[3] = view.data[i + 3];
        result[j].x = *(float*)float_arr;

        float_arr[0] = view.data[i + 4];
        float_arr[1] = view.data[i + 5];
        float_arr[2] = view.data[i + 6];
        float_arr[3] = view.data[i + 7];
        result[j].y = *(float*)float_arr;

        float_arr[0] = view.data[i + 8];
        float_arr[1] = view.data[i + 9];
        float_arr[2] = view.data[i + 10];
        float_arr[3] = view.data[i + 11];
        result[j].z = *(float*)float_arr;

        j++;
    }

    return result;
}

static view_t parse_mesh_data(const json_t* json,
                              const json_node_t* node,
                              const char* attribute,
                              const chunk_t binary)
{
    // node has an index from the accessors array
    // accessors array contains indices from the buffer views array
    // buffers array holds data used to access the binary chunks

    const json_node_t* accessors = json_find_node(json, 1, JSON_ACCESSORS);
    const json_node_t* views     = json_find_node(json, 1, JSON_BUFFER_VIEWS);

    const json_node_t* index     = json_find_child(node, attribute);
    const json_node_t* accessor  = json_find_index(accessors, index->uinteger);
    
    index                        = json_find_child(accessor, JSON_BUFFER_VIEW);
    const json_node_t* count     = json_find_child(accessor, JSON_COUNT);
    const json_node_t* view      = json_find_index(views, index->uinteger);
    // const json_node_t* buffer    = json_find_child(view, JSON_BUFFER);
    const json_node_t* offset    = json_find_child(view, JSON_BYTE_OFFSET);
    const json_node_t* length    = json_find_child(view, JSON_BYTE_LEN);

    uint32_t start = offset ? offset->uinteger : 0;

    view_t result = {
        .data = &binary.data[start],
        .size = length->uinteger,
        .count = count->uinteger,
    };

    return result;
}

static texture_t* parse_material_texture(const json_t* json, const json_node_t* node, chunk_t binary)
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

    // const json_node_t* buffer    = json_find_child(view, JSON_BUFFER);
    const json_node_t* offset    = json_find_child(view, JSON_BYTE_OFFSET);
    const json_node_t* length    = json_find_child(view, JSON_BYTE_LEN);

    uint32_t start = offset ? offset->uinteger : 0;

    return parse_png(&binary.data[start], length->uinteger);
}

static mesh_t* parse_meshes(const json_t* json, const chunk_t binary)
{
    const json_node_t* scenes       = json_find_node(json, 1, JSON_SCENES);
    const json_node_t* nodes        = json_find_node(json, 1, JSON_NODES);
    const json_node_t* meshes       = json_find_node(json, 1, JSON_MESHES);
    const json_node_t* materials    = json_find_node(json, 1, JSON_MATERIALS);
    const json_node_t* scene        = json_find_index(scenes, 0);
    const json_node_t* scene_nodes  = json_find_child(scene, JSON_NODES);
    const json_node_t* node_index   = json_find_index(scene_nodes, 0);

    // get mesh
    const json_node_t* node         = json_find_index(nodes, node_index->uinteger);
    const json_node_t* index        = json_find_child(node, JSON_MESH);
    const json_node_t* mesh         = json_find_index(meshes, index->uinteger);
    const json_node_t* primitives   = json_find_child(mesh, JSON_PRIMITIVES);

    // parse vertex data
    const json_node_t* primitive    = json_find_index(primitives, 0);                     // TODO: allow multiple primitives
    const json_node_t* attributes   = json_find_child(primitive, JSON_ATTRIBUTES);

    view_t indices_view             = parse_mesh_data(json, primitive, JSON_INDICES, binary);
    view_t positions_view           = parse_mesh_data(json, attributes, JSON_POSITION, binary);
    view_t normals_view             = parse_mesh_data(json, attributes, JSON_NORMAL, binary);
    view_t tex_coords_view          = parse_mesh_data(json, attributes, JSON_TEXCOORD_0, binary);

    uint32_t* indices               = create_indices_array(indices_view);
    vec_t* positions                = create_vec_array(positions_view);
    vec_t* normals                  = create_vec_array(normals_view);
    vec_t* tex_coords               = create_vec_array(tex_coords_view);

    // parse material data
    index                           = json_find_child(primitive, JSON_MATERIAL);
    const json_node_t* material     = json_find_index(materials, index->uinteger);
    const json_node_t* pbr          = json_find_child(material, JSON_PBR);
    const json_node_t* albedo       = json_find_child(pbr, JSON_ALBEDO_TEX);
    const json_node_t* metallic     = json_find_child(pbr, JSON_MR_TEX);
    const json_node_t* normal       = json_find_child(material, JSON_NORMAL_TEX);
    const json_node_t* occlusion    = json_find_child(material, JSON_OCCLUSION_TEX);

    texture_t* albedo_tex           = parse_material_texture(json, albedo, binary);
    texture_t* metallic_tex         = parse_material_texture(json, metallic, binary);
    texture_t* normal_tex           = parse_material_texture(json, normal, binary);
    texture_t* occlusion_tex        = parse_material_texture(json, occlusion, binary);

    return mesh_new("name", 
                    positions,
                    tex_coords,
                    normals,
                    indices,
                    positions_view.count,
                    tex_coords_view.count,
                    normals_view.count,
                    indices_view.count,
                    albedo_tex,
                    metallic_tex,
                    normal_tex,
                    occlusion_tex);
}

/********************/
/* public functions */
/********************/

scene_t* parse_scene(const char* file_path)
{
    file_t* file = file_new(file_path);
    scene_t* scene = malloc(sizeof(scene_t));

    parse_header(file);

    // parse json and binary chunks
    const chunk_t json_chunk = parse_chunk(file);
    const chunk_t binary = parse_chunk(file);

    // parse json tree
    json_t* json = json_new(json_chunk.data, json_chunk.size);
    validate_glb_scene(json);

    // scene->dir_light
    // scene->point_light
    scene->mesh = parse_meshes(json, binary);
    vec_t cam_pos = vec_new(2.f, 2.f, 2.f);
    scene->camera = camera_new(cam_pos,
                               0.610866f,
                               -2.356194f,
                               (float)M_PI_2,
                               1.f,
                               100.f,
                               1.3333f);

    // free buffers
    json_free(json);
    file_free(file);

    return scene;
}