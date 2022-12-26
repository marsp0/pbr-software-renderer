#include "test_json.h"

#include <stddef.h>
#include "test_utils.h"

#include "../parsers/json.h"

void test_string(void)
{
    const unsigned char buff[] = "{ \"key1\":  \"val12\"}";
    
    json_t* json = json_new(buff, 19);
    ASSERT_INT(2, json->nodes_size);
    ASSERT_INT(9, json->strings_size);
    ASSERT_STRING("key1val12", json->strings, 8);

    json_node_t node = json->nodes[0];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(3, node.type);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_INT(4, node.key_size);
    ASSERT_INT(5, node.string_size);
    ASSERT_INT(2, node.type);
    ASSERT_STRING("key1", node.key, node.key_size);
    ASSERT_STRING("val12", node.string, node.string_size);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    json_free(json);
}

void test_multiple_strings(void)
{
    const unsigned char buff[] = "{ \"key1\":  \"val12\", \"key2\": \"val2\"}";

    json_t* json = json_new(buff, 35);
    ASSERT_INT(3, json->nodes_size);
    ASSERT_INT(17, json->strings_size);
    ASSERT_STRING("key1val12key2val2", json->strings, 8);

    json_node_t node = json->nodes[0];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(3, node.type);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_INT(4, node.key_size);
    ASSERT_INT(5, node.string_size);
    ASSERT_INT(2, node.type);
    ASSERT_STRING("key1", node.key, node.key_size);
    ASSERT_STRING("val12", node.string, node.string_size);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[2], node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[2];
    ASSERT_INT(4, node.key_size);
    ASSERT_INT(4, node.string_size);
    ASSERT_INT(2, node.type);
    ASSERT_STRING("key2", node.key, node.key_size);
    ASSERT_STRING("val2", node.string, node.string_size);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    json_free(json);
}

void test_number(void)
{
    const unsigned char buff[] = "{ \"key1\":  123 }";
    
    json_t* json = json_new(buff, 16);
    ASSERT_INT(2, json->nodes_size);
    ASSERT_INT(4, json->strings_size);
    ASSERT_STRING("key1", json->strings, 4);

    json_node_t node = json->nodes[0];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(3, node.type);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_INT(4, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(123, node.integer);
    ASSERT_INT(0, node.type);
    ASSERT_STRING("key1", node.key, node.key_size);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    json_free(json);
}

void test_multiple_numbers(void)
{
    const unsigned char buff[] = "{ \"key1\":  123 , \"key2\": -3432}";
    
    json_t* json = json_new(buff, 31);
    ASSERT_INT(3, json->nodes_size);
    ASSERT_INT(8, json->strings_size);
    ASSERT_STRING("key1key2", json->strings, 8);

    json_node_t node = json->nodes[0];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(3, node.type);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_INT(4, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(123, node.integer);
    ASSERT_INT(0, node.type);
    ASSERT_STRING("key1", node.key, node.key_size);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[2], node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[2];
    ASSERT_INT(4, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(-3432, node.integer);
    ASSERT_INT(0, node.type);
    ASSERT_STRING("key2", node.key, node.key_size);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    json_free(json);
}

void test_real(void)
{
    const unsigned char buff[] = "{ \"key1\":  0.321 }";
    
    json_t* json = json_new(buff, 18);
    ASSERT_INT(2, json->nodes_size);
    ASSERT_INT(4, json->strings_size);
    ASSERT_STRING("key1", json->strings, 4);

    json_node_t node = json->nodes[0];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(3, node.type);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_INT(4, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_FLOAT(0.321f, node.real);
    ASSERT_INT(1, node.type);
    ASSERT_STRING("key1", node.key, node.key_size);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    json_free(json);
}

void test_multiple_reals(void)
{
    const unsigned char buff[] = "{ \"key1\": 0.321 , \"key2\": -34.32}";
    
    json_t* json = json_new(buff, 33);
    ASSERT_INT(3, json->nodes_size);
    ASSERT_INT(8, json->strings_size);
    ASSERT_STRING("key1key2", json->strings, 8);

    json_node_t node = json->nodes[0];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(3, node.type);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_INT(4, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_FLOAT(0.321f, node.real);
    ASSERT_INT(1, node.type);
    ASSERT_STRING("key1", node.key, node.key_size);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[2], node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[2];
    ASSERT_INT(4, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_FLOAT(-34.32f, node.real);
    ASSERT_INT(1, node.type);
    ASSERT_STRING("key2", node.key, node.key_size);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    json_free(json);
}

void test_nested_objects(void)
{
    const unsigned char buff[] = "{ \"key1\": 0.321, \"key2\": { \"inner_key1\": \"some string\"}}";

    json_t* json = json_new(buff, 56);
    ASSERT_INT(4, json->nodes_size);
    ASSERT_INT(29, json->strings_size);
    ASSERT_STRING("key1key2inner_key1some string", json->strings, json->strings_size);

    json_node_t node = json->nodes[0];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(3, node.type);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_INT(4, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_FLOAT(0.321f, node.real);
    ASSERT_INT(1, node.type);
    ASSERT_STRING("key1", node.key, node.key_size);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[2], node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[2];
    ASSERT_INT(4, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(3, node.type);
    ASSERT_STRING("key2", node.key, node.key_size);
    ASSERT_POINTER(&json->nodes[3], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[3];
    ASSERT_INT(10, node.key_size);
    ASSERT_INT(11, node.string_size);
    ASSERT_INT(2, node.type);
    ASSERT_STRING("inner_key1", node.key, node.key_size);
    ASSERT_STRING("some string", node.string, node.string_size)
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[2], node.parent);

    json_free(json);
}

void test_empty_containers(void)
{
    const unsigned char buff[] = "{ \"key1\":  [  ]  , \"key2.2\"  : {} }";
    
    json_t* json = json_new(buff, 36);
    ASSERT_INT(3, json->nodes_size);
    ASSERT_INT(10, json->strings_size);
    ASSERT_STRING("key1key2.2", json->strings, json->strings_size);

    json_node_t node = json->nodes[0];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(3, node.type);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_INT(4, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(4, node.type);
    ASSERT_STRING("key1", node.key, node.key_size);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[2], node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[2];
    ASSERT_INT(6, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(3, node.type);
    ASSERT_STRING("key2.2", node.key, node.key_size);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    json_free(json);
}

void test_int_array(void)
{
    const unsigned char buff[] = "{ \"key1\":  [ 1, 2, 3 , 3] }";
    
    json_t* json = json_new(buff, 27);
    ASSERT_INT(6, json->nodes_size);
    ASSERT_INT(4, json->strings_size);
    ASSERT_STRING("key1", json->strings, json->strings_size);

    json_node_t node = json->nodes[0];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(3, node.type);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_INT(4, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(4, node.type);
    ASSERT_STRING("key1", node.key, node.key_size);
    ASSERT_POINTER(&json->nodes[2], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[2];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(0, node.type);
    ASSERT_INT(1, node.integer);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[3], node.next);
    ASSERT_POINTER(&json->nodes[1], node.parent);

    node = json->nodes[3];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(0, node.type);
    ASSERT_INT(2, node.integer);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[4], node.next);
    ASSERT_POINTER(&json->nodes[1], node.parent);

    node = json->nodes[4];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(0, node.type);
    ASSERT_INT(3, node.integer);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[5], node.next);
    ASSERT_POINTER(&json->nodes[1], node.parent);

    node = json->nodes[5];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(0, node.type);
    ASSERT_INT(3, node.integer);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[1], node.parent);

    json_free(json);
}

void test_string_array(void)
{
    const unsigned char buff[] = "{ \"key1\":  [ \"one\", \"two\"] }";
    
    json_t* json = json_new(buff, 28);
    ASSERT_INT(4, json->nodes_size);
    ASSERT_INT(10, json->strings_size);
    ASSERT_STRING("key1onetwo", json->strings, json->strings_size);

    json_node_t node = json->nodes[0];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(3, node.type);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_INT(4, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(4, node.type);
    ASSERT_STRING("key1", node.key, node.key_size);
    ASSERT_POINTER(&json->nodes[2], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[2];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(3, node.string_size);
    ASSERT_INT(2, node.type);
    ASSERT_STRING("one", node.string, node.string_size);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[3], node.next);
    ASSERT_POINTER(&json->nodes[1], node.parent);

    node = json->nodes[3];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(3, node.string_size);
    ASSERT_INT(2, node.type);
    ASSERT_STRING("two", node.string, node.string_size);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[1], node.parent);

    json_free(json);
}

void test_mixed_array(void)
{
    const unsigned char buff[] = "{ \"key1\": [ \"one\", 2, { \"key3\" : [ 0.23, 3222.432 ]}] }";
    
    json_t* json = json_new(buff, 55);
    ASSERT_INT(8, json->nodes_size);
    ASSERT_INT(11, json->strings_size);
    ASSERT_STRING("key1onekey3", json->strings, json->strings_size);

    json_node_t node = json->nodes[0];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(3, node.type);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_INT(4, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(4, node.type);
    ASSERT_STRING("key1", node.key, node.key_size);
    ASSERT_POINTER(&json->nodes[2], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[2];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(3, node.string_size);
    ASSERT_INT(2, node.type);
    ASSERT_STRING("one", node.string, node.string_size);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[3], node.next);
    ASSERT_POINTER(&json->nodes[1], node.parent);

    node = json->nodes[3];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(0, node.type);
    ASSERT_INT(2, node.integer);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[4], node.next);
    ASSERT_POINTER(&json->nodes[1], node.parent);

    node = json->nodes[4];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(3, node.type);
    ASSERT_POINTER(&json->nodes[5], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[1], node.parent);

    node = json->nodes[5];
    ASSERT_INT(4, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(4, node.type);
    /*TODO LOOK*/
    ASSERT_STRING("key3", node.string, node.string_size);
    ASSERT_POINTER(&json->nodes[6], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[4], node.parent);

    node = json->nodes[6];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(1, node.type);
    ASSERT_FLOAT(0.23f, node.real);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[7], node.next);
    ASSERT_POINTER(&json->nodes[5], node.parent);

    node = json->nodes[7];
    ASSERT_INT(0, node.key_size);
    ASSERT_INT(0, node.string_size);
    ASSERT_INT(1, node.type);
    ASSERT_FLOAT(3222.432f, node.real);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[5], node.parent);

    json_free(json);
}

void test_find_node(void)
{
    const unsigned char buff[] = "{ \"key1\": 0.321, \"key2\": { \"inner_key1\": \"some string\"}}";

    json_t* json = json_new(buff, 56);
    json_node_t* node = json_find_node(json, 1, "key1");

    ASSERT_POINTER(&json->nodes[1], node);
    ASSERT_INT(4, node->key_size);
    ASSERT_INT(0, node->string_size);
    ASSERT_INT(1, node->type);
    ASSERT_FLOAT(0.321f, node->real);
    ASSERT_POINTER(NULL, node->child);
    ASSERT_POINTER(&json->nodes[2], node->next);
    ASSERT_POINTER(&json->nodes[0], node->parent);

    json_free(json);
}

void test_find_node_nested(void)
{
    const unsigned char buff[] = "{ \"key1\": 0.321, \"key2\": { \"inner_key1\": \"some string\"}}";

    json_t* json = json_new(buff, 56);
    json_node_t* node = json_find_node(json, 2, "key2", "inner_key1");

    ASSERT_POINTER(&json->nodes[3], node);
    ASSERT_INT(10, node->key_size);
    ASSERT_INT(11, node->string_size);
    ASSERT_INT(2, node->type);
    ASSERT_STRING("inner_key1", node->key, node->key_size);
    ASSERT_STRING("some string", node->string, node->string_size);
    ASSERT_POINTER(NULL, node->child);
    ASSERT_POINTER(NULL, node->next);
    ASSERT_POINTER(&json->nodes[2], node->parent);

    json_free(json);
}

void test_find_node_missing(void)
{
    const unsigned char buff[] = "{ \"key1\": 0.321, \"key2\": { \"inner_key1\": \"some string\"}}";

    json_t* json = json_new(buff, 56);

    json_node_t* node = json_find_node(json, 2, "doesnt_exist", "inner_key1");
    ASSERT_POINTER(NULL, node);

    node = json_find_node(json, 2, "key2", "doesnt_exist");
    ASSERT_POINTER(NULL, node);

    json_free(json);
}

void test_find_node_invalid(void)
{
    const unsigned char buff[] = "{ \"key1\": 0.321, \"key2\": { \"inner_key1\": \"some string\"}}";

    json_t* json = json_new(buff, 56);

    json_node_t* node = json_find_node(json, 0, "doesnt_exist", "inner_key1");
    ASSERT_POINTER(NULL, node);

    json_free(json);
}

void test_json(void)
{
    test_string();
    test_multiple_strings();
    test_number();
    test_multiple_numbers();
    test_real();
    test_multiple_reals();
    test_nested_objects();
    test_empty_containers();
    test_int_array();
    test_string_array();
    test_mixed_array();
    test_find_node();
    test_find_node_nested();
    test_find_node_missing();
    test_find_node_invalid();
}