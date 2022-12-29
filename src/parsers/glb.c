#include "glb.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../constants.h"
#include "png.h"
#include "json.h"
#include "glb_validator.h"

/*
 * GLTF 2 specification - https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html
 */

static uint32_t cursor = 0;

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

mesh_t* parse_scene(const char* file_name)
{

    /* open file */
    FILE* file = fopen(file_name, "r");
    assert(file != NULL);

    /* get file size */
    fseek(file, 0, SEEK_END);
    uint32_t file_size = (uint32_t)ftell(file);
    fseek(file, 0, SEEK_SET);

    /* copy file contents into buffer */
    unsigned char* buffer = malloc(file_size);
    fread(buffer, sizeof(unsigned char), file_size, file);

    /* close file as its no longer needed */
    fclose(file);

    printf("Allocating %.02fMB for %s\n", file_size / 1024.0 / 1024.0, file_name);

    cursor = 0;
    /*TODO: do we need this header ? */
    header_t header = parse_header(buffer);
    printf("%d\n", header.size);
    chunk_t json_chunk = parse_chunk(buffer);
    /*chunk_t binary_chunk = parse_chunk(buffer);*/
    json_t* json = json_new(json_chunk.data, json_chunk.size);

    validate_glb(json);

    /*TODO: Validate json content
     * geom type - float + vec3
     * needs normal map
     * needs rougthness map
     * needs metallic map
     */

    mesh_t* result = NULL;

    /* free the buffer */
    free(json);
    free(buffer);

    return result;
}