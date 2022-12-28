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
    struct json_node_t* parent;
    char*               key;
    uint32_t            key_size;
    uint32_t            string_size;
    json_type_e         type;

    union
    {
        float   real;
        int32_t integer;
        char*   string;
    };

} json_node_t;

typedef struct
{
    char*        strings;
    json_node_t* nodes;
    uint32_t     strings_size;
    uint32_t     nodes_size;
} json_t;

json_t*         json_new(const unsigned char* input, uint32_t input_size);
void            json_free(json_t* json);
json_node_t*    json_find_node(json_t* json, uint32_t arg_count, ...);
json_node_t*    json_find_child(json_node_t* json, const char* key);
json_node_t*    json_find_array_element(json_node_t* json, uint32_t index);