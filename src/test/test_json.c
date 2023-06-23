#include "test_json.h"

#include <stddef.h>

#include "test_utils.h"
#include "../parsers/json.h"

static void test_string(void)
{
    const unsigned char buff[] = "{ \"key1\":  \"val12\"}";
    
    json_t* json = json_new(buff, sizeof(buff) - 1);
    ASSERT_EQUAL(2, json->nodes_size);
    ASSERT_EQUAL(9, json->strings_size);
    ASSERT_STRING("key1val12", json->strings, 8);

    json_node_t node = json->nodes[0];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(JSON_OBJECT, node.type);
    ASSERT_EQUAL(1, node.size);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(5, node.size);
    ASSERT_EQUAL(JSON_STRING, node.type);
    ASSERT_STRING("key1", node.key, 4);
    ASSERT_STRING("val12", node.string, 5);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    json_free(json);
}

static void test_empty_string(void)
{
    const unsigned char buff[] = "{ \"key1\":  \"\", \"\":2}";

    json_t* json = json_new(buff, sizeof(buff) - 1);
    ASSERT_EQUAL(3, json->nodes_size);
    ASSERT_EQUAL(4, json->strings_size);
    ASSERT_STRING("key1", json->strings, 4);

    json_node_t node = json->nodes[0];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(JSON_OBJECT, node.type);
    ASSERT_EQUAL(2, node.size);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(JSON_STRING, node.type);
    ASSERT_STRING("key1", node.key, 4);
    // ASSERT_STRING("", node.string, 0);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[2], node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[2];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(JSON_NUMBER, node.type);
    // ASSERT_STRING("", node.key, 0);
    ASSERT_EQUAL(2, node.integer);
    ASSERT_EQUAL(2, node.uinteger);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    json_free(json);
}

static void test_string_with_quote(void)
{
    // { "key\"1":  "\"va\"12\""}
    const unsigned char buff[] = "{ \"key\\\"1\":  \"\\\"va\\\"12\\\"\"}";
    
    json_t* json = json_new(buff, sizeof(buff) - 1);
    ASSERT_EQUAL(2, json->nodes_size);
    ASSERT_EQUAL(12, json->strings_size);
    ASSERT_STRING("key\"1\"va\"12\"", json->strings, 12);

    json_node_t node = json->nodes[0];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(JSON_OBJECT, node.type);
    ASSERT_EQUAL(1, node.size);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_EQUAL(5, node.key_size);
    ASSERT_EQUAL(7, node.size);
    ASSERT_EQUAL(JSON_STRING, node.type);
    ASSERT_STRING("key\"1", node.key, 5);
    ASSERT_STRING("\"va\"12\"", node.string, 7);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    json_free(json);
}

static void test_string_with_escape_sequences(void)
{
    const unsigned char buff[] = "{ \"key1\": \"\\\\a\\\"b/c\\bd\\fe\\nf\\rg\\t\"}";

    json_t* json = json_new(buff, sizeof(buff) - 1);
    
    ASSERT_EQUAL(2, json->nodes_size);
    ASSERT_EQUAL(19, json->strings_size);
    ASSERT_STRING("key1\\a\"b/c\bd\fe\nf\rg\t", json->strings, 19);

    json_node_t node = json->nodes[0];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(JSON_OBJECT, node.type);
    ASSERT_EQUAL(1, node.size);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(15, node.size);
    ASSERT_EQUAL(JSON_STRING, node.type);
    ASSERT_STRING("key1", node.key, 4);
    ASSERT_STRING("\\a\"b/c\bd\fe\nf\rg\t", node.string, 7);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    json_free(json);
}

static void test_multiple_strings(void)
{
    const unsigned char buff[] = "{ \"key1\":  \"val12\", \"key2\": \"val2\"}";

    json_t* json = json_new(buff, sizeof(buff) - 1);
    ASSERT_EQUAL(3, json->nodes_size);
    ASSERT_EQUAL(17, json->strings_size);
    ASSERT_STRING("key1val12key2val2", json->strings, 8);

    json_node_t node = json->nodes[0];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(JSON_OBJECT, node.type);
    ASSERT_EQUAL(2, node.size);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(5, node.size);
    ASSERT_EQUAL(JSON_STRING, node.type);
    ASSERT_STRING("key1", node.key, 4);
    ASSERT_STRING("val12", node.string, 5);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[2], node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[2];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(4, node.size);
    ASSERT_EQUAL(JSON_STRING, node.type);
    ASSERT_STRING("key2", node.key, 4);
    ASSERT_STRING("val2", node.string, 4);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    json_free(json);
}

static void test_integer(void)
{
    const unsigned char buff[] = "{ \"key1\":  123 }";
    
    json_t* json = json_new(buff, sizeof(buff) - 1);
    ASSERT_EQUAL(2, json->nodes_size);
    ASSERT_EQUAL(4, json->strings_size);
    ASSERT_STRING("key1", json->strings, 4);

    json_node_t node = json->nodes[0];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(JSON_OBJECT, node.type);
    ASSERT_EQUAL(1, node.size);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(123, node.integer);
    ASSERT_EQUAL(123, node.uinteger);
    ASSERT_EQUAL(JSON_NUMBER, node.type);
    ASSERT_STRING("key1", node.key, 4);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    json_free(json);
}

static void test_multiple_integers(void)
{
    const unsigned char buff[] = "{ \"key1\":  123 , \"key2\": -3432}";
    
    json_t* json = json_new(buff, sizeof(buff) - 1);
    ASSERT_EQUAL(3, json->nodes_size);
    ASSERT_EQUAL(8, json->strings_size);
    ASSERT_STRING("key1key2", json->strings, 8);

    json_node_t node = json->nodes[0];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(JSON_OBJECT, node.type);
    ASSERT_EQUAL(2, node.size);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(123, node.integer);
    ASSERT_EQUAL(123, node.uinteger);
    ASSERT_EQUAL(JSON_NUMBER, node.type);
    ASSERT_STRING("key1", node.key, 4);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[2], node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[2];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(-3432, node.integer);
    ASSERT_EQUAL(JSON_NUMBER, node.type);
    ASSERT_STRING("key2", node.key, 4);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    json_free(json);
}

static void test_real(void)
{
    const unsigned char buff[] = "{ \"key1\":  0.321 }";
    
    json_t* json = json_new(buff, sizeof(buff) - 1);
    ASSERT_EQUAL(2, json->nodes_size);
    ASSERT_EQUAL(4, json->strings_size);
    ASSERT_STRING("key1", json->strings, 4);

    json_node_t node = json->nodes[0];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(JSON_OBJECT, node.type);
    ASSERT_EQUAL(1, node.size);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(0.321f, node.real);
    ASSERT_EQUAL(JSON_REAL, node.type);
    ASSERT_STRING("key1", node.key, 4);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    json_free(json);
}

static void test_multiple_reals(void)
{
    const unsigned char buff[] = "{ \"key1\": 0.321 , \"key2\": -34.32}";
    
    json_t* json = json_new(buff, sizeof(buff) - 1);
    ASSERT_EQUAL(3, json->nodes_size);
    ASSERT_EQUAL(8, json->strings_size);
    ASSERT_STRING("key1key2", json->strings, 8);

    json_node_t node = json->nodes[0];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(JSON_OBJECT, node.type);
    ASSERT_EQUAL(2, node.size);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(0.321f, node.real);
    ASSERT_EQUAL(JSON_REAL, node.type);
    ASSERT_STRING("key1", node.key, 4);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[2], node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[2];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(-34.32f, node.real);
    ASSERT_EQUAL(JSON_REAL, node.type);
    ASSERT_STRING("key2", node.key, 4);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    json_free(json);
}

static void test_nested_objects(void)
{
    const unsigned char buff[] = "{ \"key1\": 0.321, \"key2\": { \"inner_key1\": \"some string\"}}";

    json_t* json = json_new(buff, sizeof(buff) - 1);
    ASSERT_EQUAL(4, json->nodes_size);
    ASSERT_EQUAL(29, json->strings_size);
    ASSERT_STRING("key1key2inner_key1some string", json->strings, 29);

    json_node_t node = json->nodes[0];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(JSON_OBJECT, node.type);
    ASSERT_EQUAL(2, node.size);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(0.321f, node.real);
    ASSERT_EQUAL(JSON_REAL, node.type);
    ASSERT_STRING("key1", node.key, 4);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[2], node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[2];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(JSON_OBJECT, node.type);
    ASSERT_EQUAL(1, node.size);
    ASSERT_STRING("key2", node.key, 4);
    ASSERT_POINTER(&json->nodes[3], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[3];
    ASSERT_EQUAL(10, node.key_size);
    ASSERT_EQUAL(11, node.size);
    ASSERT_EQUAL(JSON_STRING, node.type);
    ASSERT_STRING("inner_key1", node.key, 10);
    ASSERT_STRING("some string", node.string, 11)
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[2], node.parent);

    json_free(json);
}

static void test_empty_containers(void)
{
    const unsigned char buff[] = "{ \"key1\":  [  ]  , \"key2.2\"  : {} }";
    
    json_t* json = json_new(buff, sizeof(buff) - 1);
    ASSERT_EQUAL(3, json->nodes_size);
    ASSERT_EQUAL(10, json->strings_size);
    ASSERT_STRING("key1key2.2", json->strings, 10);

    json_node_t node = json->nodes[0];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(JSON_OBJECT, node.type);
    ASSERT_EQUAL(2, node.size);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(JSON_ARRAY, node.type);
    ASSERT_EQUAL(0, node.size);
    ASSERT_STRING("key1", node.key, 4);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[2], node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[2];
    ASSERT_EQUAL(6, node.key_size);
    ASSERT_EQUAL(JSON_OBJECT, node.type);
    ASSERT_EQUAL(0, node.size);
    ASSERT_STRING("key2.2", node.key, 6);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    json_free(json);
}

static void test_int_array(void)
{
    const unsigned char buff[] = "{ \"key1\":  [ 1, 2, 3 , 3] }";
    
    json_t* json = json_new(buff, sizeof(buff) - 1);
    ASSERT_EQUAL(6, json->nodes_size);
    ASSERT_EQUAL(4, json->strings_size);
    ASSERT_STRING("key1", json->strings, 4);

    json_node_t node = json->nodes[0];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(JSON_OBJECT, node.type);
    ASSERT_EQUAL(1, node.size);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(JSON_ARRAY, node.type);
    ASSERT_EQUAL(4, node.size);
    ASSERT_STRING("key1", node.key, 4);
    ASSERT_POINTER(&json->nodes[2], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[2];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(JSON_NUMBER, node.type);
    ASSERT_EQUAL(1, node.integer);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[3], node.next);
    ASSERT_POINTER(&json->nodes[1], node.parent);

    node = json->nodes[3];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(JSON_NUMBER, node.type);
    ASSERT_EQUAL(2, node.integer);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[4], node.next);
    ASSERT_POINTER(&json->nodes[1], node.parent);

    node = json->nodes[4];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(JSON_NUMBER, node.type);
    ASSERT_EQUAL(3, node.integer);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[5], node.next);
    ASSERT_POINTER(&json->nodes[1], node.parent);

    node = json->nodes[5];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(JSON_NUMBER, node.type);
    ASSERT_EQUAL(3, node.integer);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[1], node.parent);

    json_free(json);
}

static void test_string_array(void)
{
    const unsigned char buff[] = "{ \"key1\":  [ \"one\", \"two\"] }";
    
    json_t* json = json_new(buff, sizeof(buff) - 1);
    ASSERT_EQUAL(4, json->nodes_size);
    ASSERT_EQUAL(10, json->strings_size);
    ASSERT_STRING("key1onetwo", json->strings, 10);

    json_node_t node = json->nodes[0];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(JSON_OBJECT, node.type);
    ASSERT_EQUAL(1, node.size);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(JSON_ARRAY, node.type);
    ASSERT_EQUAL(2, node.size);
    ASSERT_STRING("key1", node.key, 4);
    ASSERT_POINTER(&json->nodes[2], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[2];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(3, node.size);
    ASSERT_EQUAL(JSON_STRING, node.type);
    ASSERT_STRING("one", node.string, 3);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[3], node.next);
    ASSERT_POINTER(&json->nodes[1], node.parent);

    node = json->nodes[3];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(3, node.size);
    ASSERT_EQUAL(JSON_STRING, node.type);
    ASSERT_STRING("two", node.string, 3);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[1], node.parent);

    json_free(json);
}

static void test_mixed_array(void)
{
    const unsigned char buff[] = "{ \"key1\": [ \"one\", 2, { \"key3\" : [ 0.23, 3222.432 ]}] }";
    
    json_t* json = json_new(buff, sizeof(buff) - 1);
    ASSERT_EQUAL(8, json->nodes_size);
    ASSERT_EQUAL(11, json->strings_size);
    ASSERT_STRING("key1onekey3", json->strings, 11);

    json_node_t node = json->nodes[0];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(JSON_OBJECT, node.type);
    ASSERT_EQUAL(1, node.size);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(JSON_ARRAY, node.type);
    ASSERT_EQUAL(3, node.size);
    ASSERT_STRING("key1", node.key, 4);
    ASSERT_POINTER(&json->nodes[2], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[2];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(3, node.size);
    ASSERT_EQUAL(JSON_STRING, node.type);
    ASSERT_STRING("one", node.string, 3);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[3], node.next);
    ASSERT_POINTER(&json->nodes[1], node.parent);

    node = json->nodes[3];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(JSON_NUMBER, node.type);
    ASSERT_EQUAL(2, node.integer);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[4], node.next);
    ASSERT_POINTER(&json->nodes[1], node.parent);

    node = json->nodes[4];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(JSON_OBJECT, node.type);
    ASSERT_EQUAL(1, node.size);
    ASSERT_POINTER(&json->nodes[5], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[1], node.parent);

    node = json->nodes[5];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(JSON_ARRAY, node.type);
    ASSERT_EQUAL(2, node.size);
    ASSERT_STRING("key3", node.key, 4);
    ASSERT_POINTER(&json->nodes[6], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[4], node.parent);

    node = json->nodes[6];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(JSON_REAL, node.type);
    ASSERT_EQUAL(0.23f, node.real);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[7], node.next);
    ASSERT_POINTER(&json->nodes[5], node.parent);

    node = json->nodes[7];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(JSON_REAL, node.type);
    ASSERT_EQUAL(3222.432f, node.real);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[5], node.parent);

    json_free(json);
}

static void test_bool(void)
{
    const unsigned char buff[] = "{ \"key1\":  false, \"key2\":true}";
    
    json_t* json = json_new(buff, sizeof(buff) - 1);
    ASSERT_EQUAL(3, json->nodes_size);
    ASSERT_EQUAL(8, json->strings_size);
    ASSERT_STRING("key1key2", json->strings, 4);

    json_node_t node = json->nodes[0];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(JSON_OBJECT, node.type);
    ASSERT_EQUAL(2, node.size);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(JSON_BOOL, node.type);
    ASSERT_EQUAL(false, node.boolean);
    ASSERT_STRING("key1", node.key, 4);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[2], node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[2];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(JSON_BOOL, node.type);
    ASSERT_EQUAL(true, node.boolean);
    ASSERT_STRING("key2", node.key, 4);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    json_free(json);
}

static void test_null(void)
{
    const unsigned char buff[] = "{ \"key1\":  false, \"key2\":null}";
    
    json_t* json = json_new(buff, sizeof(buff) - 1);
    ASSERT_EQUAL(3, json->nodes_size);
    ASSERT_EQUAL(8, json->strings_size);
    ASSERT_STRING("key1key2", json->strings, 4);

    json_node_t node = json->nodes[0];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(JSON_OBJECT, node.type);
    ASSERT_EQUAL(2, node.size);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(JSON_BOOL, node.type);
    ASSERT_EQUAL(false, node.boolean);
    ASSERT_STRING("key1", node.key, 4);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(&json->nodes[2], node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    node = json->nodes[2];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(0, node.size);
    ASSERT_EQUAL(JSON_NULL, node.type);
    ASSERT_STRING("key2", node.key, 4);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    json_free(json);
}

static void test_brackets_inside_string(void)
{
    const unsigned char buff[] = "{ \"key1\":  \"{}l[]\"}";
    
    json_t* json = json_new(buff, sizeof(buff) - 1);
    ASSERT_EQUAL(2, json->nodes_size);
    ASSERT_EQUAL(9, json->strings_size);
    ASSERT_STRING("key1{}l[]", json->strings, 8);

    json_node_t node = json->nodes[0];
    ASSERT_EQUAL(0, node.key_size);
    ASSERT_EQUAL(JSON_OBJECT, node.type);
    ASSERT_EQUAL(1, node.size);
    ASSERT_POINTER(&json->nodes[1], node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(NULL, node.parent);

    node = json->nodes[1];
    ASSERT_EQUAL(4, node.key_size);
    ASSERT_EQUAL(5, node.size);
    ASSERT_EQUAL(JSON_STRING, node.type);
    ASSERT_STRING("key1", node.key, 4);
    ASSERT_STRING("{}l[]", node.string, 5);
    ASSERT_POINTER(NULL, node.child);
    ASSERT_POINTER(NULL, node.next);
    ASSERT_POINTER(&json->nodes[0], node.parent);

    json_free(json);
}

static void test_find_node(void)
{
    const unsigned char buff[] = "{ \"key1\": 0.321, \"key2\": { \"inner_key1\": \"some string\"}}";

    json_t* json = json_new(buff, sizeof(buff) - 1);
    const json_node_t* node = json_find_node(json, 1, "key1");

    ASSERT_POINTER(&json->nodes[1], node);
    ASSERT_EQUAL(4, node->key_size);
    ASSERT_EQUAL(0, node->size);
    ASSERT_EQUAL(JSON_REAL, node->type);
    ASSERT_EQUAL(0.321f, node->real);
    ASSERT_POINTER(NULL, node->child);
    ASSERT_POINTER(&json->nodes[2], node->next);
    ASSERT_POINTER(&json->nodes[0], node->parent);

    json_free(json);
}

static void test_find_node_nested(void)
{
    const unsigned char buff[] = "{ \"key1\": 0.321, \"key2\": { \"inner_key1\": \"some string\"}}";

    json_t* json = json_new(buff, sizeof(buff) - 1);
    const json_node_t* node = json_find_node(json, 2, "key2", "inner_key1");

    ASSERT_POINTER(&json->nodes[3], node);
    ASSERT_EQUAL(10, node->key_size);
    ASSERT_EQUAL(11, node->size);
    ASSERT_EQUAL(JSON_STRING, node->type);
    ASSERT_STRING("inner_key1", node->key, 10);
    ASSERT_STRING("some string", node->string, 11);
    ASSERT_POINTER(NULL, node->child);
    ASSERT_POINTER(NULL, node->next);
    ASSERT_POINTER(&json->nodes[2], node->parent);

    json_free(json);
}

static void test_find_node_missing(void)
{
    const unsigned char buff[] = "{ \"key1\": 0.321, \"key2\": { \"inner_key1\": \"some string\"}}";

    json_t* json = json_new(buff, sizeof(buff) - 1);

    const json_node_t* node = json_find_node(json, 2, "doesnt_exist", "inner_key1");
    ASSERT_POINTER(NULL, node);

    node = json_find_node(json, 2, "key2", "doesnt_exist");
    ASSERT_POINTER(NULL, node);

    json_free(json);
}

static void test_find_node_invalid(void)
{
    const unsigned char buff[] = "{ \"key1\": 0.321, \"key2\": { \"inner_key1\": \"some string\"}}";

    json_t* json = json_new(buff, sizeof(buff) - 1);

    const json_node_t* node = json_find_node(json, 0, "doesnt_exist", "inner_key1");
    ASSERT_POINTER(NULL, node);

    json_free(json);
}

static void test_find_child(void)
{
    const unsigned char buff[] = "{ \"key1\": 0.321, \"key2\": { \"inner_key1\": \"some string\"}}";

    json_t* json = json_new(buff, sizeof(buff) - 1);

    const json_node_t* node = json_find_child(&json->nodes[0], "key2");
    ASSERT_POINTER(&json->nodes[2], node);

    node = json_find_child(&json->nodes[0], "key3");
    ASSERT_POINTER(NULL, node);

    json_free(json);
}

static void test_find_array_element(void)
{
    const unsigned char buff[] = "{ \"key1\":  [ \"one\", \"two\"] }";
    
    json_t* json = json_new(buff, sizeof(buff) - 1);

    const json_node_t* node = json_find_index(&json->nodes[1], 1);
    ASSERT_POINTER(&json->nodes[3], node);

    node = json_find_index(&json->nodes[0], 14);
    ASSERT_POINTER(NULL, node);

    json_free(json);
}

void test_json(void)
{
    TEST_CASE(test_string);
    TEST_CASE(test_empty_string);
    TEST_CASE(test_string_with_quote);
    TEST_CASE(test_string_with_escape_sequences);
    TEST_CASE(test_multiple_strings);
    TEST_CASE(test_integer);
    TEST_CASE(test_multiple_integers);
    TEST_CASE(test_real);
    TEST_CASE(test_multiple_reals);
    TEST_CASE(test_nested_objects);
    TEST_CASE(test_empty_containers);
    TEST_CASE(test_int_array);
    TEST_CASE(test_string_array);
    TEST_CASE(test_mixed_array);
    TEST_CASE(test_bool);
    TEST_CASE(test_null);
    TEST_CASE(test_brackets_inside_string);
    TEST_CASE(test_find_node);
    TEST_CASE(test_find_node_nested);
    TEST_CASE(test_find_node_missing);
    TEST_CASE(test_find_node_invalid);
    TEST_CASE(test_find_child);
    TEST_CASE(test_find_array_element);
}