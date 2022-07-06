#include "vector_array.h"

#include <stdlib.h>

vec_array* vec_array_new()
{
    vec_array* result = malloc(sizeof(vec_array));
    
    result->size = 0;
    result->capacity = 128;
    result->data = malloc(result->capacity * sizeof(vec));

    return result;
}

void vec_array_add(vec_array* array, vec element)
{
    array->data[array->size] = element;
    array->size++;
    if (array->size == array->capacity)
    {
        array->capacity *= 2;
        vec* tmp = realloc(array->data, array->capacity);
        if (tmp == NULL)
        {
            free(array->data);
            exit(1);
        }
        array->data = tmp;
    }
}

void vec_array_destroy(vec_array* array)
{
    free(array->data);
    free(array);
}