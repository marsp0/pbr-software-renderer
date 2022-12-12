#include "json.h"

#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../constants.h"

/* TODO: describe what is happening here */

typedef enum
{
    TYPE_NONE = 0,
    TYPE_NUMBER,
    TYPE_STRING,
    TYPE_OBJECT,
    TYPE_ARRAY
} type_e;   /* mostly used for arrays */

typedef enum
{
    STATUS_NONE = 0,
    STATUS_BEFORE_KEY,
    STATUS_IN_KEY,
    STATUS_BEFORE_VALUE,
    STATUS_IN_VALUE,
} status_e;     /* used internally to facilitate parsing */

typedef struct
{
    int     key_start;      /* starting byte of key */
    int     key_size;       /* size of key */
    int     value_start;    /* starting byte of value. same use as child in JSON_ARRAY */
    int     value_size;     /* size of value. indicates array length in JSON_ARRAY. Not used in JSON_OBJECT */
    int     child;          /* index in tokens array of the first child of the node*/
    int 	sibling;        /* index in tokens array of the next item on the same level */
    type_e 	type;
} token_t;

static uint32_t cursor                      = 0;
static uint32_t buffer_size                 = 0;
static const unsigned char* buffer          = NULL;

static uint32_t token_index                 = 0; /* size of tokens */
static token_t tokens[JSON_TOKENS_CAPACITY] = { 0 }; /* holds all parsed json tokens */

static int parse_number(int index)
{
    tokens[index].type = TYPE_NUMBER;
    tokens[index].value_start = cursor;

    while (isdigit(buffer[cursor]) || buffer[cursor] == '.')
    {
        cursor++;
    }

    tokens[index].value_size = cursor - tokens[index].value_start;
    return 0;
}

static int parse_string(int index)
{
    cursor++;
    tokens[index].value_start = cursor;
    tokens[index].type = TYPE_STRING;

    while (buffer[cursor] != '"')
    {
        cursor++;
    }

    tokens[index].value_size = cursor - tokens[index].value_start;
    return 0;
}

static void parse_key(int index)
{
    status_e status = STATUS_BEFORE_KEY;

    while (status != STATUS_BEFORE_VALUE)
    {
        if (buffer[cursor] == '"' && status == STATUS_BEFORE_KEY)
        {
            status = STATUS_IN_KEY;
            tokens[index].key_start = cursor + 1;
        }
        else if (buffer[cursor] == '"' && status == STATUS_IN_KEY)
        {
            status = STATUS_BEFORE_VALUE;
            tokens[index].key_size = cursor - tokens[index].key_start;
        }
        cursor++;
    }
}

static void parse_value(int index);

static int parse_object(int index)
{
    int child = token_index;
    status_e status = STATUS_BEFORE_KEY;

    tokens[index].value_start = cursor;
    tokens[index].child = token_index;
    tokens[index].type = TYPE_OBJECT;

    token_index++;
    assert(token_index < JSON_TOKENS_CAPACITY);
    
    while (buffer[cursor] != '}' && cursor < buffer_size)
    {

        if (status == STATUS_BEFORE_KEY && buffer[cursor] == '"')
        {
            status = STATUS_BEFORE_VALUE;
            parse_key(child);
        }
        else if (status == STATUS_BEFORE_VALUE)
        {
            status = STATUS_NONE;
            parse_value(child);
        }
        
        if (status == STATUS_NONE && buffer[cursor] == ',')
        {
            status = STATUS_BEFORE_KEY;
            tokens[child].sibling = token_index;
            child = token_index;
            token_index++;
            assert(token_index < JSON_TOKENS_CAPACITY);
        }
        cursor++;
    }

    return 0;
}

static int parse_array(int index)
{
    int child = token_index;
    tokens[index].value_start = token_index;
    tokens[index].child = token_index;
    tokens[index].type = TYPE_ARRAY;

    token_index++;
    assert(token_index < JSON_TOKENS_CAPACITY);

    while (buffer[cursor] != ']' && cursor < buffer_size)
    {
        /* common work needed for all types */
        if (buffer[cursor] == '{' || buffer[cursor] == '"' || isdigit(buffer[cursor]))
        {
            tokens[index].value_size++;
         
            tokens[child].key_start = tokens[index].key_start;
            tokens[child].key_size = tokens[index].key_size;
        }

        if (buffer[cursor] == '{')
        {
            parse_object(child);
        }
        else if (buffer[cursor] == '"')
        {
            parse_string(child);
        }
        else if (isdigit(buffer[cursor]))
        {
            parse_number(child);
        }

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

static void parse_value(int index)
{
    int next = 1;
    while (next)
    {
        if (isdigit(buffer[cursor]))
        {
            next = parse_number(index);
        }
        else if (buffer[cursor] == '"')
        {
            next = parse_string(index);
        }
        else if (buffer[cursor] == '{')
        {
            next = parse_object(index);
        }
        else if (buffer[cursor] == '[')
        {
            next = parse_array(index);
        }

        if (next)
        {
            cursor++;
        }
    }
}

/*
 * parse_json_buffer - takes an already allocated buffer of json data, parses it and populates
 *                     the statically allocated *tokens* array;
 */
void parse_json(const unsigned char* buf, int buf_size)
{
    /* clear tokens */
    memset(tokens, 0, sizeof(tokens));

    cursor = 0;
    buffer_size = buf_size;
    buffer = buf;
    token_index = 0;
    parse_object(token_index);
    printf("Parsed %d JSON tokens\n", token_index);
}