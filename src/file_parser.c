#include "file_parser.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

void parse_obj(const char* file_name)
{
    const int LINE_SIZE = 256;
    char line[LINE_SIZE];
    int items;

    FILE* file = fopen(file_name, "r");
    assert(file != NULL);

    while(fgets(line, LINE_SIZE, file) != NULL)
    {
        if (strncmp(line, "o ", 1) == 0)
        {
            
        }
    }

    fclose(file);
    if (!feof(file) && ferror(file))
    {
        printf("Error occured while loading %s\n", file_name);
        exit(1);
    }
}