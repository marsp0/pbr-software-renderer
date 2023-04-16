#include "file.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/********************
 *  Notes
 *
 ********************/

/********************/
/*      defines     */
/********************/

/********************/
/* static variables */
/********************/

/********************/
/* static functions */
/********************/

/********************/
/* public functions */
/********************/

file_t* file_new(const char* file_path)
{
    file_t* file = malloc(sizeof(file_t));

    // open file
    FILE* handle = fopen(file_path, "r");
    assert(handle != NULL);

    // get file size
    fseek(handle, 0, SEEK_END);
    uint32_t file_size = (uint32_t)ftell(handle);
    fseek(handle, 0, SEEK_SET);

    // copy file contents into buffer
    unsigned char* buffer = malloc(file_size);
    fread(buffer, sizeof(unsigned char), file_size, handle);

    // close file as its no longer needed
    fclose(handle);

    file->data = buffer;
    file->cursor = 0;
    file->size = file_size;

    return file;
}

void file_free(file_t* file)
{
    free(file->data);
    free(file);
}