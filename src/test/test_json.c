#include "test_json.h"

#include "test_utils.h"

#include "../parsers/json.h"

void test_parse_string()
{
	const unsigned char buff[] = "{ \"key1\":  \"val12\"}";
	
	json_t* json = json_new(buff, 19);
	ASSERT_INT(1, json->nodes_size);
	ASSERT_INT(9, json->strings_size);
	ASSERT_STRING("key1val12", json->strings, 8);

	json_node_t node = json->nodes[0];
	ASSERT_INT(4, node.name_size);
	ASSERT_INT(5, node.string_size);
	ASSERT_INT(2, node.type);
	ASSERT_STRING("key1", node.name, node.name_size);
	ASSERT_STRING("val12", node.string, node.string_size);

	json_free(json);
}

void test_parse_multiple_strings()
{
	const unsigned char buff[] = "{ \"key1\":  \"val12\", \"key2\": \"val2\"}";

	json_t* json = json_new(buff, 35);
	ASSERT_INT(2, json->nodes_size);
	ASSERT_INT(17, json->strings_size);
	ASSERT_STRING("key1val12key2val2", json->strings, 8);

	json_node_t node = json->nodes[0];
	ASSERT_INT(4, node.name_size);
	ASSERT_INT(5, node.string_size);
	ASSERT_INT(2, node.type);
	ASSERT_STRING("key1", node.name, node.name_size);
	ASSERT_STRING("val12", node.string, node.string_size);
	ASSERT_STRING("key2", node.next->name, node.next->name_size);

	node = json->nodes[1];
	ASSERT_INT(4, node.name_size);
	ASSERT_INT(4, node.string_size);
	ASSERT_INT(2, node.type);
	ASSERT_STRING("key2", node.name, node.name_size);
	ASSERT_STRING("val2", node.string, node.string_size);

	json_free(json);
}

void test_parse_number()
{
	const unsigned char buff[] = "{ \"key1\":  123 }";
	
	json_t* json = json_new(buff, 16);
	ASSERT_INT(1, json->nodes_size);
	ASSERT_INT(4, json->strings_size);
	ASSERT_STRING("key1", json->strings, 4);

	json_node_t node = json->nodes[0];
	ASSERT_INT(4, node.name_size);
	ASSERT_INT(0, node.string_size);
	ASSERT_INT(123, node.num);
	ASSERT_INT(0, node.type);
	ASSERT_STRING("key1", node.name, node.name_size);

	json_free(json);
}

void test_parse_multiple_numbers()
{
	const unsigned char buff[] = "{ \"key1\":  123 , \"key2\": 3432}";
	
	json_t* json = json_new(buff, 30);
	ASSERT_INT(2, json->nodes_size);
	ASSERT_INT(8, json->strings_size);
	ASSERT_STRING("key1key2", json->strings, 8);

	json_node_t node = json->nodes[0];
	ASSERT_INT(4, node.name_size);
	ASSERT_INT(0, node.string_size);
	ASSERT_INT(123, node.num);
	ASSERT_INT(0, node.type);
	ASSERT_STRING("key1", node.name, node.name_size);
	ASSERT_STRING("key2", node.next->name, node.next->name_size);

	node = json->nodes[1];
	ASSERT_INT(4, node.name_size);
	ASSERT_INT(0, node.string_size);
	ASSERT_INT(3432, node.num);
	ASSERT_INT(0, node.type);
	ASSERT_STRING("key2", node.name, node.name_size);

	json_free(json);
}

void test_parse_real()
{
	const unsigned char buff[] = "{ \"key1\":  0.321 }";
	
	json_t* json = json_new(buff, 18);
	ASSERT_INT(1, json->nodes_size);
	ASSERT_INT(4, json->strings_size);
	ASSERT_STRING("key1", json->strings, 4);

	json_node_t node = json->nodes[0];
	ASSERT_INT(4, node.name_size);
	ASSERT_INT(0, node.string_size);
	ASSERT_FLOAT(0.321f, node.real);
	ASSERT_INT(1, node.type);
	ASSERT_STRING("key1", node.name, node.name_size);

	json_free(json);
}

void test_parse_multiple_reals()
{
	const unsigned char buff[] = "{ \"key1\": 0.321 , \"key2\": 34.32}";
	
	json_t* json = json_new(buff, 32);
	ASSERT_INT(2, json->nodes_size);
	ASSERT_INT(8, json->strings_size);
	ASSERT_STRING("key1key2", json->strings, 8);

	json_node_t node = json->nodes[0];
	ASSERT_INT(4, node.name_size);
	ASSERT_INT(0, node.string_size);
	ASSERT_FLOAT(0.321f, node.real);
	ASSERT_INT(1, node.type);
	ASSERT_STRING("key1", node.name, node.name_size);
	ASSERT_STRING("key2", node.next->name, node.next->name_size);

	node = json->nodes[1];
	ASSERT_INT(4, node.name_size);
	ASSERT_INT(0, node.string_size);
	ASSERT_FLOAT(34.32f, node.real);
	ASSERT_INT(1, node.type);
	ASSERT_STRING("key2", node.name, node.name_size);

	json_free(json);
}

void test_json()
{
	test_parse_string();
	test_parse_multiple_strings();
	test_parse_number();
	test_parse_multiple_numbers();
	test_parse_real();
	test_parse_multiple_reals();
}