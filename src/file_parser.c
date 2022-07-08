#include "file_parser.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static 

void parse_obj(const char* file_name)
{

    FILE* file = fopen(file_name, "r");
    assert(file != NULL);

    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char* buffer = malloc(file_size);
    fread(buffer, sizeof(unsigned char), file_size, file);

    const int LINE_SIZE = 256;
    char line[LINE_SIZE];
    while(fgets(line, LINE_SIZE, buffer) != NULL)
    {
        if (strncmp(line, "o ", 1) == 0)
        {
            
        }
    }

    fclose(file);

}