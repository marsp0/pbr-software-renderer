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
static unsigned char* strings      = NULL;
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

static bool is_valid(void)
{
    /* TODO: improve validation */

    cursor = 0;
    
    int i = 0;
    bool is_valid = true;
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
            if (stack[--i] != t)
            {
                is_valid = false;
                break;
            }
        }

        cursor++;
    }

    cursor = 0;

    return is_valid;
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

        // string allocation
        if (c == '"' && status == BEFORE_KEY)
        {
            status = IN_KEY;
        }
        else if (c == '"' && status == IN_KEY) 
        {
            status = buffer[cursor - 1] != '\\' ? BEFORE_KEY : IN_KEY;
        }
        else if (status == IN_KEY)
        {
            ssize++;
        }

        // node allocation
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

static void parse_string(unsigned char** value, uint32_t* size)
{

    unsigned char c;

    bool run        = true;
    bool memcopy    = true;
    uint32_t start  = 0;
    uint32_t end    = 0;
    status_e status = BEFORE_KEY;

    // find beginning and end of string
    while (run)
    {
        c = buffer[cursor];

        if (c == '"' && status == BEFORE_KEY)
        {
            start = cursor + 1;
            status = IN_KEY;
        }
        else if (c == '"' && status == IN_KEY)
        {
            if (buffer[cursor - 1] == '\\')
            {
                memcopy = false;
            }
            else
            {
                end = cursor;
                run = false;
            }
        }

        cursor++;
    }

    // copy string into string buffer
    if (memcopy)
    {
        *size = end - start;
        *value = &strings[string_index];

        memcpy(*value, &buffer[start], *size);
        string_index += *size;

        return;
    }

    // copy manually due to escaped quotes
    uint32_t count = 0;
    *value = &strings[string_index];

    for (uint32_t i = start; i < end; i++)
    {

        if (buffer[i] == '"' && buffer[i - 1] == '\\')
        {
            string_index--;
            count--;
        }

        strings[string_index] = buffer[i];
        string_index++;
        count++;
    }

    *size = count;
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
    else if (buffer[cursor] == 'f' || buffer[cursor] == 't')
    {
        nodes[index].type = JSON_BOOL;
        nodes[index].boolean = strncmp(&buffer[cursor], "true", 4) == 0 ? true : false;
        cursor += nodes[index].boolean ? 4 : 5;
    }
    else if (strncmp(&buffer[cursor], "null", 4) == 0)
    {
        nodes[index].type = JSON_NULL;
        cursor += 4;
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
    // set static vars
    buffer = input;
    buffer_size = input_size;
    result = malloc(sizeof(json_t));
    
    // validate input buffer
    if (!is_valid())
    {
        return NULL;
    }

    // allocate memory
    allocate();

    // set more static vars
    cursor = 0;
    node_index = 0;
    string_index = 0;
    nodes = result->nodes;
    strings = result->strings;

    // parse buffer
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
    const json_node_t* curr = count > 0 ? &json->nodes[0] : NULL;

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

    const json_node_t* curr = json->child;
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

    const json_node_t* curr = json->child;

    for (uint32_t i = 0; i < index; i++)
    {
        curr = curr ? curr->next : NULL;
    }

    return curr;
}