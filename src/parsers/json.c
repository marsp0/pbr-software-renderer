#include "json.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../constants.h"

static uint32_t cursor             = 0;
static uint32_t buffer_size        = 0;
static const unsigned char* buffer = NULL;

static json_t* json                = NULL;
static json_node_t* nodes          = NULL;
static uint32_t node_index         = 0;
static char* strings               = NULL;
static uint32_t string_index       = 0;

typedef enum
{
    NONE,
    BEFORE_KEY,
    IN_KEY,
    BEFORE_VAL,
    IN_VAL,
} status_e;

static bool is_real()
{
    uint32_t cur = cursor;
    bool is_real = false;

    while(isdigit(buffer[cur]) || buffer[cur] == '.')
    {

        if (buffer[cur] == '.')
        {
            is_real = true;
        }
        cur++;
    }

    return is_real;
}

static bool skip_if_empty(unsigned char end)
{
    uint32_t cur = cursor;
    while(isspace(buffer[cur]))
    {
        cur++;
    }

    if (buffer[cur] == end)
    {
        cursor = ++cur;
        return true;
    }

    return false;
}

static void skip_whitespace()
{
    unsigned char c = buffer[cursor];
    while(isspace(c) || c == ',' || c == ':')
    {
        cursor++;
        c = buffer[cursor];
    }
}

static void validate()
{
    cursor = 0;
    
    int i = 0;
    unsigned char c;
    unsigned char t;
    unsigned char stack[JSON_DEPTH] = { 0 };

    while(cursor < buffer_size)
    {
        c = buffer[cursor];
        
        if (c == '{' || c == '[')
        {
            stack[i++] = c;
        }

        if (c == '}' || c == ']')
        {
            t = c == '}' ? '{' : '[';
            assert(stack[--i] == t);
        }

        cursor++;
    }

    assert(i == 0);
    
    cursor = 0;
}

static void allocate_string_buffer()
{
    cursor = 0;

    uint32_t size = 0;
    status_e status = BEFORE_KEY;

    while(cursor < buffer_size)
    {

        if (buffer[cursor] == '"' && status == BEFORE_KEY)
        {
            status = IN_KEY;
        }
        else if (buffer[cursor] == '"' && status == IN_KEY)
        {
            status = BEFORE_KEY;
        }
        else if (status == IN_KEY)
        {
            size++;
        }

        cursor++;
    }

    printf("size: %d\n", size);
    json->strings = malloc(size + 1);
    json->strings_size = size;
    json->strings[size] = 0;
    memset(json->strings, 0, size);

    cursor = 0;
}

static void allocate_node_buffer()
{
    cursor = 0;

    int count = 0;

    unsigned char c;
    unsigned char t;
    while(cursor < buffer_size)
    {
        c = buffer[cursor];
        if (c == ',' || c == ']' || c == '}')
        {
            count += 1;
        }
        else if (c == '[' || c == '{')
        {
            cursor++;
            t = c == '[' ? ']' : '}';
            skip_if_empty(t);
        }
        cursor++;
    }

    json->nodes = malloc(count * sizeof(json_node_t));
    json->nodes_size = count;
    memset(json->nodes, 0, count * sizeof(json_node_t));

    printf("node count: %d\n", count);

    cursor = 0;
}

static void parse_number(uint32_t index)
{
    nodes[index].type = JSON_NUMBER;
    nodes[index].integer = atoi(&buffer[cursor]);

    /* update cursor to current position */
    while(isdigit(buffer[cursor]))
    {
        cursor++;
    }

}

static void parse_real(uint32_t index)
{
    nodes[index].type = JSON_REAL;
    nodes[index].real = (float)atof(&buffer[cursor]);

    /* update cursor to current position */
    while(isdigit(buffer[cursor]) || buffer[cursor] == '.')
    {
        cursor++;
    }
}

static void parse_string(char** result, uint32_t* size)
{
    uint32_t start = 0;
    status_e status = BEFORE_KEY;

    while(*size == 0)
    {
        if (buffer[cursor] == '"' && status == BEFORE_KEY)
        {
            start = cursor + 1;
            status = IN_KEY;
        }
        else if (buffer[cursor] == '"' && status == IN_KEY)
        {
            *size = cursor - start;
            *result = &(strings[string_index]);

            memcpy(*result, &buffer[start], *size);
            string_index += *size;

            printf("%s--\n", strings);
        }
        cursor++;
    }
}

static void parse_string_key(uint32_t index)
{
    parse_string(&(nodes[index].name), &(nodes[index].name_size));
}

static void parse_string_value(uint32_t index)
{
    nodes[index].type = JSON_STRING;
    parse_string(&(nodes[index].string), &(nodes[index].string_size));
}

static void parse_object(uint32_t index);
static void parse_array(uint32_t index);
static void parse_value(uint32_t index)
{
    skip_whitespace();
    if (buffer[cursor] == '"')
    {
        parse_string_value(index);
    }
    else if (isdigit(buffer[cursor]))
    {
        is_real() ? parse_real(index) : parse_number(index);
    }
    else if (buffer[cursor] == '{')
    {
        parse_object(index);
    }
    else if (buffer[cursor] == '[')
    {
        parse_array(index);
    }
    skip_whitespace();
}

static void parse_array(uint32_t index)
{
    json_node_t* prev = NULL;
    uint32_t current = 0;
    nodes[index].type = JSON_ARRAY;

    cursor++;

    if (skip_if_empty(']'))
    {
        return;
    }

    while(buffer[cursor] != ']')
    {
        current = node_index;
        node_index++;

        parse_value(current);
        
        if (prev)
        {
            prev->next = &nodes[current];
        }
        else
        {
            nodes[index].child = &nodes[current];
        }
        prev = &nodes[current];
    }

    cursor++;
}

static void parse_object(uint32_t index)
{
    json_node_t* prev = NULL;
    status_e status = BEFORE_KEY;
    nodes[index].type = JSON_OBJECT;
    cursor++;

    while(buffer[cursor] != '}')
    {
        skip_whitespace();
        if (buffer[cursor] == '"' && status == BEFORE_KEY)
        {
            parse_string_key(node_index);
            status = BEFORE_VAL;

            if (prev)
            {
                prev->next = &nodes[node_index];
            }
            else if (!prev && index != 0) /* first child and no self reference (applicable only to top level object) */
            {
                nodes[index].child = &nodes[node_index];
            }

            prev = &nodes[node_index];
        }
        else if (status == BEFORE_VAL)
        {
            parse_value(node_index++);
            status = BEFORE_KEY;
        }
    }

    cursor++;
}

json_t* json_new(const unsigned char* input, uint32_t input_size)
{
    buffer = input;
    buffer_size = input_size;
    json = malloc(sizeof(json_t));
    
    validate();
    allocate_string_buffer();
    allocate_node_buffer();

    nodes = json->nodes;
    strings = json->strings;
    node_index = 0;
    string_index = 0;

    parse_object(0);

    return json;
}

void json_free(json_t* json)
{
    free(json->strings);
    free(json->nodes);
    free(json);
}