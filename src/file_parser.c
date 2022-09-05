#include "file_parser.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

#include "constants.h"
#include "mesh.h"
#include "parsers/png.h"

/*
 * GLTF 2 specification - https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html
 */

static int cursor = 0;

/*
 * GLB
 */

typedef struct
{
    uint32_t magic; /* 0x46546C67 */
    uint32_t version;
    uint32_t size;
} glb_header_t;

typedef struct
{
    uint32_t size;
    uint32_t type; /* 0x4E4F534A or 0x004E4942 */
    const unsigned char* data;
} glb_chunk_t;


static uint32_t glb_parse_int(const unsigned char* buffer, int n);

static glb_header_t glb_parse_header(const unsigned char* buffer)
{
    glb_header_t header;
    header.magic = glb_parse_int(buffer, 4);
    header.version = glb_parse_int(buffer, 4);
    header.size = glb_parse_int(buffer, 4);

    assert(header.version == 2);
    assert(header.magic == 0x46546C67);

    return header;
}

static glb_chunk_t glb_parse_chunk(const unsigned char* buffer)
{
    glb_chunk_t chunk;
    chunk.size = glb_parse_int(buffer, 4);
    chunk.type = glb_parse_int(buffer, 4);
    chunk.data = &buffer[cursor];

    cursor += chunk.size;

    /* assert that chunk is either bin or json */
    assert(chunk.type == 0x4E4F534A || chunk.type == 0x004E4942);

    return chunk;
}

static uint32_t glb_parse_int(const unsigned char* buffer, int n)
{
    uint32_t result = 0;
    for (int i = cursor; i < cursor + n; i++)
    {
        result += buffer[i] << ((i - cursor) * 8);
    }
    cursor += n;
    return result;
}

/*
 * JSON
 */

typedef enum
{
    JSON_NONE = 0,
    JSON_NUMBER,
    JSON_STRING,
    JSON_OBJECT,
    JSON_ARRAY
} json_type_e;   /* mostly used for arrays */

typedef enum
{
    JSON_STATUS_NONE = 0,
    JSON_STATUS_BEFORE_KEY,
    JSON_STATUS_IN_KEY,
    JSON_STATUS_BEFORE_VALUE,
    JSON_STATUS_IN_VALUE,
} json_status_e;     /* used internally to facilitate parsing */

typedef struct
{
    int         key_start;      /* starting byte of key */
    int         key_size;       /* size of key */
    int         value_start;    /* starting byte of value. same use as child in JSON_ARRAY */
    int         value_size;     /* size of value. indicates array length in JSON_ARRAY. Not used in JSON_OBJECT */
    int         child;          /* index in tokens array of the first child of the node*/
    int         sibling;        /* index in tokens array of the next item on the same level */
    json_type_e type;
} json_token_t;

static int token_index = 0; /* size of tokens */
static int json_size = 0;
static json_token_t tokens[JSON_TOKENS_CAPACITY]; /* holds all parsed json tokens */


static void json_parse_key(const unsigned char* buffer, int index);
static void json_parse_value(const unsigned char* buffer, int index);
static int  json_parse_number(const unsigned char* buffer, int index);
static int  json_parse_string(const unsigned char* buffer, int index);
static int  json_parse_object(const unsigned char* buffer, int index);
static int  json_parse_array(const unsigned char* buffer, int index);

/*
 * parse_json_buffer - takes an already allocated buffer of json data, parses it and populates
 *                     the statically allocated *tokens* array;
 */
static void parse_json_buffer(glb_chunk_t chunk)
{
    /* clear tokens */
    memset(tokens, 0, sizeof(tokens));

    cursor = 0;
    token_index = 0;
    json_size = chunk.size;
    json_parse_object(chunk.data, token_index);
    printf("Parsed %d JSON tokens\n", token_index);
}

static void json_parse_key(const unsigned char* buffer, int index)
{
    json_status_e status = JSON_STATUS_BEFORE_KEY;

    while (status != JSON_STATUS_BEFORE_VALUE)
    {
        if (buffer[cursor] == '"' && status == JSON_STATUS_BEFORE_KEY)
        {
            status = JSON_STATUS_IN_KEY;
            tokens[index].key_start = cursor + 1;
        }
        else if (buffer[cursor] == '"' && status == JSON_STATUS_IN_KEY)
        {
            status = JSON_STATUS_BEFORE_VALUE;
            tokens[index].key_size = cursor - tokens[index].key_start;
        }
        cursor++;
    }
}

static void json_parse_value(const unsigned char* buffer, int index)
{
    int next = 1;
    while (next)
    {
        if (isdigit(buffer[cursor]))
            next = json_parse_number(buffer, index);

        else if (buffer[cursor] == '"')
            next = json_parse_string(buffer, index);

        else if (buffer[cursor] == '{')
            next = json_parse_object(buffer, index);

        else if (buffer[cursor] == '[')
            next = json_parse_array(buffer, index);

        if (next)
            cursor++;
    }
}

static int json_parse_array(const unsigned char* buffer, int index)
{
    int child = token_index;
    tokens[index].value_start = token_index;
    tokens[index].child = token_index;
    tokens[index].type = JSON_ARRAY;

    token_index++;
    assert(token_index < JSON_TOKENS_CAPACITY);

    while (buffer[cursor] != ']' && cursor < json_size)
    {
        /* common work needed for all types */
        if (buffer[cursor] == '{' || buffer[cursor] == '"' || isdigit(buffer[cursor]))
        {
            tokens[index].value_size++;
         
            tokens[child].key_start = tokens[index].key_start;
            tokens[child].key_size = tokens[index].key_size;
        }

        if (buffer[cursor] == '{')
            json_parse_object(buffer, child);

        else if (buffer[cursor] == '"')
            json_parse_string(buffer, child);

        else if (isdigit(buffer[cursor]))
            json_parse_number(buffer, child);

        if (buffer[cursor] == ',')
        {
            tokens[child].sibling = token_index;
            child = token_index;
            token_index++;
            assert(token_index < JSON_TOKENS_CAPACITY);
        }
        cursor++;
    }

    return 0;
}

static int json_parse_object(const unsigned char* buffer, int index)
{
    int child = token_index;
    json_status_e status = JSON_STATUS_BEFORE_KEY;

    tokens[index].value_start = cursor;
    tokens[index].child = token_index;
    tokens[index].type = JSON_OBJECT;

    token_index++;
    assert(token_index < JSON_TOKENS_CAPACITY);
    
    while (buffer[cursor] != '}' && cursor < json_size)
    {

        if (status == JSON_STATUS_BEFORE_KEY && buffer[cursor] == '"')
        {
            status = JSON_STATUS_BEFORE_VALUE;
            json_parse_key(buffer, child);
        }
        else if (status == JSON_STATUS_BEFORE_VALUE)
        {
            status = JSON_STATUS_NONE;
            json_parse_value(buffer, child);
        }
        
        if (status == JSON_STATUS_NONE && buffer[cursor] == ',')
        {
            status = JSON_STATUS_BEFORE_KEY;
            tokens[child].sibling = token_index;
            child = token_index;
            token_index++;
            assert(token_index < JSON_TOKENS_CAPACITY);
        }
        cursor++;
    }

    return 0;
}

static int json_parse_number(const unsigned char* buffer, int index)
{
    tokens[index].type = JSON_NUMBER;
    tokens[index].value_start = cursor;

    while (isdigit(buffer[cursor]) || buffer[cursor] == '.')
        cursor++;

    tokens[index].value_size = cursor - tokens[index].value_start;
    return 0;
}

static int json_parse_string(const unsigned char* buffer, int index)
{
    cursor++;
    tokens[index].value_start = cursor;
    tokens[index].type = JSON_STRING;

    while (buffer[cursor] != '"')
        cursor++;

    tokens[index].value_size = cursor - tokens[index].value_start;
    return 0;
}


/*
 * parse_scene
 */

int parse_scene(const char* file_name, mesh_t* meshes[], int meshes_capacity)
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
    const glb_header_t header = glb_parse_header(buffer);
    const glb_chunk_t json = glb_parse_chunk(buffer);
    const glb_chunk_t binary = glb_parse_chunk(buffer);

    parse_json_buffer(json);

    parse_png(binary.data, 2165850);

    /* free the buffer */
    free(buffer);
}