#pragma once

#include <stdint.h>

typedef enum
{
    JSON_NUMBER,
    JSON_REAL,
    JSON_STRING,
    JSON_OBJECT,
    JSON_ARRAY,
} json_type_e;

typedef struct json_node_t
{
    struct json_node_t* child;
    struct json_node_t* next;
    char*               name;
    uint32_t            name_size;
    uint32_t            string_size;
    json_type_e         type;

    union
    {
        float  real;
        int    integer;
        char*  string;
    };

} json_node_t;

typedef struct
{
    char*        strings;
    json_node_t* nodes;
    uint32_t     strings_size;
    uint32_t     nodes_size;
} json_t;

json_t* json_new(const unsigned char* input, uint32_t input_size);
void    json_free(json_t* data);