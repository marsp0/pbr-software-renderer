#include "file_parser.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "constants.h"
#include "mesh.h"

/*
 * GLB structs
 */

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
    unsigned char* data;
} chunk_t;

/*
 * JSON structs
 */

typedef enum
{
    JSON_NUMBER = 0,
    JSON_STRING,
    JSON_OBJECT,
    JSON_ARRAY
} type_e;   /* mostly used for arrays */

typedef enum
{
    JSON_STATUS_NONE = 0,
    JSON_STATUS_BEFORE_KEY,
    JSON_STATUS_IN_KEY,
    JSON_STATUS_BEFORE_VALUE,
    JSON_STATUS_IN_VALUE,
} status_e;     /* used internally to facilitate parsing */

typedef struct
{
    int     key_start;      /* starting byte of key */
    int     key_size;       /* size of key */
    int     value_start;    /* starting byte of value. same use as child in JSON_ARRAY */
    int     value_size;     /* size of value. indicates array length in JSON_ARRAY. Not used in JSON_OBJECT */
    int     child;          /* index in tokens array of the first child of the node*/
    int     sibling;        /* index in tokens array of the next item on the same level */
    type_e  type;
} token_t;

static int cursor = 0;
static int token_index = 0; /* size of tokens */
static int json_size = 0;
static token_t tokens[JSON_TOKENS_CAPACITY]; /* holds all parsed json tokens */

/*
 * GLB functions
 */

static int parse_int(unsigned char* buffer, int n);

static header_t parse_header(unsigned char* buffer)
{
    header_t header;
    header.magic = parse_int(&buffer[cursor], 4);
    header.version = parse_int(&buffer[cursor + 4], 4);
    header.size = parse_int(&buffer[cursor + 8], 4);
    
    cursor += 12;

    assert(header.version == 2);
    assert(header.magic == 0x46546C67);

    return header;
}

static chunk_t parse_chunk(unsigned char* buffer)
{
    chunk_t chunk;
    chunk.size = parse_int(&buffer[cursor], 4);
    chunk.type = parse_int(&buffer[cursor + 4], 4);
    chunk.data = &buffer[cursor + 8];

    cursor += 8 + chunk.size;

    /* assert that chunk is either bin or json */
    assert(chunk.type == 0x4E4F534A || chunk.type == 0x004E4942);

    return chunk;
}

static int parse_int(unsigned char* buffer, int n)
{
    uint32_t result = 0;
    for (int i = 0; i < n; i++)
    {
        result += buffer[i] << (i * 8);
    }
    return result;
}

/*
 * JSON FUNCTIONS
 */

static void parse_key(unsigned char* buffer, int index);
static void parse_value(unsigned char* buffer, int index);
static int parse_number(unsigned char* buffer, int index);
static int parse_string(unsigned char* buffer, int index);
static int parse_object(unsigned char* buffer, int index);
static int parse_array(unsigned char* buffer, int index);


/*
 * parse_json - takes an already allocated buffer of json data, parses it and populates
 *              the statically allocated *tokens* array;
 */
static void parse_json(chunk_t chunk)
{
    cursor = 0;
    token_index = 0;
    json_size = chunk.size;
    parse_object(chunk.data, token_index);
}

static void parse_key(unsigned char* buffer, int index)
{
    status_e status = JSON_STATUS_BEFORE_KEY;

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

static void parse_value(unsigned char* buffer, int index)
{
    int next = 1;
    while (next)
    {
        if (isdigit(buffer[cursor]))
            next = parse_number(buffer, index);

        else if (buffer[cursor] == '"')
            next = parse_string(buffer, index);

        else if (buffer[cursor] == '{')
            next = parse_object(buffer, index);

        else if (buffer[cursor] == '[')
            next = parse_array(buffer, index);

        if (next)
            cursor++;
    }
}

static int parse_array(unsigned char* buffer, int index)
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
            parse_object(buffer, child);

        else if (buffer[cursor] == '"')
            parse_string(buffer, child);

        else if (isdigit(buffer[cursor]))
            parse_number(buffer, child);

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

static int parse_object(unsigned char* buffer, int index)
{
    int child = token_index;
    status_e status = JSON_STATUS_BEFORE_KEY;

    tokens[index].value_start = cursor;
    tokens[index].child = token_index;
    tokens[index].type = JSON_OBJECT;

    token_index++;
    assert(token_index < JSON_TOKENS_CAPACITY);
    
    while (buffer[cursor] != '}' && cursor < json_size)
    {
        printf("%c", buffer[cursor]);
        if (status == JSON_STATUS_BEFORE_KEY && buffer[cursor] == '"')
        {
            status = JSON_STATUS_BEFORE_VALUE;
            parse_key(buffer, child);
        }
        else if (status == JSON_STATUS_BEFORE_VALUE)
        {
            status = JSON_STATUS_NONE;
            parse_value(buffer, child);
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
    printf("%c", buffer[cursor]);
    return 0;
}

static int parse_number(unsigned char* buffer, int index)
{
    tokens[index].type = JSON_NUMBER;
    tokens[index].value_start = cursor;

    while (isdigit(buffer[cursor]) || buffer[cursor] == '.')
        cursor++;

    tokens[index].value_size = cursor - tokens[index].value_start;
    return 0;
}

static int parse_string(unsigned char* buffer, int index)
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
 * main func of module
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

    const header_t header = parse_header(buffer);
    const chunk_t json = parse_chunk(buffer);
    const chunk_t binary = parse_chunk(buffer);

    parse_json(json);

    /* free the buffer */
    free(buffer);
}