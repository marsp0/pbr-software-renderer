#include "test_vector_array.h"

#include <stdio.h>

#include "test_utils.h"
#include "../vector_array.h"


void test_vector_array_add()
{
    vec_array* array = vec_array_new();
    ASSERT_INT(array->size, 0);
    ASSERT_INT(array->capacity, 128);
    for (int i = 0; i < 129; i++)
    {
        vec_array_add(array, vec_new(0.f, 0.f, 0.f));
    }
    ASSERT_INT(array->size, 129);
    ASSERT_INT(array->capacity, 256);
}

void test_vector_array()
{
    test_vector_array_add();
}