#include "json.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "../constants.h"

/********************/
/* static variables */
/********************/

typedef enum
{
    NONE,
    BEFORE_KEY,
    IN_KEY,
    BEFORE_VAL,
    IN_VAL,
} status_e;

static uint32_t cursor             = 0;
static uint32_t buffer_size        = 0;
static const unsigned char* buffer = NULL;

static json_t* result              = NULL;
static json_node_t* nodes          = NULL;
static uint32_t node_index         = 0;
static char* strings               = NULL;
static uint32_t string_index       = 0;

/********************/
/* static functions */
/********************/

static bool is_real(void)
{
    uint32_t cur = cursor;

    unsigned char c = buffer[cur];
    while(isdigit(c) || c == '.' || c == '-')
    {
        if (c == '.')
        {
            return true;
        }
        cur++;
        c = buffer[cur];
    }

    return false;
}

static bool skip_if_empty(unsigned char end)
{
    uint32_t cur = cursor;
    cur++;                  // move past opening bracket

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

static void skip_whitespace(void)
{
    unsigned char c = buffer[cursor];
    while(isspace(c) || c == ',' || c == ':')
    {
        cursor++;
        c = buffer[cursor];
    }
}

static void validate(void)
{
    /* TODO: improve validation */

    cursor = 0;
    
    int i = 0;
    unsigned char c;
    unsigned char t;
    unsigned char stack[MAX_JSON_DEPTH] = { 0 };

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

static void allocate(void)
{
    // TODO: store repeating strings once

    cursor = 0;
    unsigned char c;
    unsigned char t;

    uint32_t ssize = 0;
    uint32_t nsize = 1; // always 1 node for the root
    status_e status = BEFORE_KEY;

    while(cursor < buffer_size)
    {
        c = buffer[cursor];

        if (c == '"' && status == BEFORE_KEY)
        {
            status = IN_KEY;
        }
        else if (c == '"' && status == IN_KEY)
        {
            status = BEFORE_KEY;
        }
        else if (status == IN_KEY)
        {
            ssize++;
        }

        if (c == ',' || c == ']' || c == '}')
        {
            nsize += 1;
        }
        else if (c == '[' || c == '{')
        {
            t = c == '[' ? ']' : '}';
            
            if (skip_if_empty(t))
            {
                continue;
            }
        }

        cursor++;
    }

    result->strings = malloc(ssize + 1);
    result->strings_size = ssize;
    result->strings[ssize] = 0;
    memset(result->strings, 0, ssize);

    result->nodes = malloc(nsize * sizeof(json_node_t));
    result->nodes_size = nsize;
    memset(result->nodes, 0, nsize * sizeof(json_node_t));

    cursor = 0;
}

static void parse_number(uint32_t index)
{
    nodes[index].type = JSON_NUMBER;
    nodes[index].integer = atoi(&buffer[cursor]);

    /* update cursor to current position */
    unsigned char c = buffer[cursor];
    while(isdigit(c) || c == '-')
    {
        cursor++;
        c = buffer[cursor];
    }

}

static void parse_real(uint32_t index)
{
    nodes[index].type = JSON_REAL;
    nodes[index].real = (float)atof(&buffer[cursor]);

    /* update cursor to current position */
    unsigned char c = buffer[cursor];
    while(isdigit(c) || c == '.' || c == '-')
    {
        cursor++;
        c = buffer[cursor];
    }
}

static void parse_string(char** value, uint32_t* size)
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
            *value = &strings[string_index];

            memcpy(*value, &buffer[start], *size);
            string_index += *size;
        }
        cursor++;
    }
}

static void parse_string_key(uint32_t index)
{
    parse_string(&nodes[index].key, &nodes[index].key_size);
}

static void parse_string_value(uint32_t index)
{
    nodes[index].type = JSON_STRING;
    parse_string(&nodes[index].string, &nodes[index].string_size);
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
    else if (isdigit(buffer[cursor]) || buffer[cursor] == '-')
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
    json_node_t* curr = NULL;
    json_node_t* parent = &nodes[index];
    parent->type = JSON_ARRAY;
    parent->size = 0;


    if (skip_if_empty(']'))
    {
        return;
    }

    cursor++;

    while(buffer[cursor] != ']')
    {
        curr = &nodes[node_index++];
        curr->parent = parent;

        parse_value(node_index - 1);

        if (prev)
        {
            prev->next = curr;
        }
        else
        {
            parent->child = curr;
        }
        prev = curr;
        parent->size++;
    }

    cursor++;
}

static void parse_object(uint32_t index)
{
    json_node_t* prev = NULL;
    json_node_t* curr = NULL;
    json_node_t* parent = &nodes[index];
    status_e status = BEFORE_KEY;
    parent->type = JSON_OBJECT;
    parent->size = 0;
    cursor++;

    while(buffer[cursor] != '}')
    {
        skip_whitespace();
        if (buffer[cursor] == '"' && status == BEFORE_KEY)
        {
            parse_string_key(node_index);
            status = BEFORE_VAL;
            curr = &nodes[node_index];
            curr->parent = parent;
            parent->size++;

            if (prev)
            {
                prev->next = curr;
            }
            else
            {
                parent->child = curr;
            }

            prev = curr;
        }
        else if (status == BEFORE_VAL)
        {
            parse_value(node_index++);
            status = BEFORE_KEY;
        }
    }

    cursor++;
}

/********************/
/* public functions */
/********************/

json_t* json_new(const unsigned char* input, uint32_t input_size)
{
    buffer = input;
    buffer_size = input_size;
    result = malloc(sizeof(json_t));
    
    validate();
    allocate();

    nodes = result->nodes;
    strings = result->strings;
    node_index = 0;
    string_index = 0;

    parse_object(node_index++);

    return result;
}

void json_free(json_t* json)
{
    free(json->strings);
    free(json->nodes);
    free(json);
}

const json_node_t* json_find_node(const json_t* json, uint32_t count, ...)
{
    va_list args;
    va_start(args, count);
    json_node_t* curr = count > 0 ? &json->nodes[0] : NULL;

    for (uint32_t i = 0; i < count; i++)
    {
        const char* key = va_arg(args, const char*);
        size_t key_len = strlen(key);
        curr = curr ? curr->child : NULL;

        while(curr)
        {
            if (key_len == curr->key_size && strncmp(key, curr->key, curr->key_size) == 0)
            {
                break;
            }

            curr = curr->next;
        }
    }

    va_end(args);
    return curr;
}

const json_node_t* json_find_child(const json_node_t* json, const char* key)
{
    if (!json || json->type != JSON_OBJECT)
    {
        return NULL;
    }

    json_node_t* curr = json->child;
    size_t key_len = strlen(key);

    while(curr)
    {
        if (key_len == curr->key_size && strncmp(key, curr->key, curr->key_size) == 0)
        {
            return curr;
        }
        
        curr = curr->next;
    }

    return NULL;
}

const json_node_t* json_find_index(const json_node_t* json, uint32_t index)
{
    if (!json || json->type != JSON_ARRAY)
    {
        return NULL;
    }

    json_node_t* curr = json->child;

    for (uint32_t i = 0; i < index; i++)
    {
        curr = curr ? curr->next : NULL;
    }

    return curr;
}