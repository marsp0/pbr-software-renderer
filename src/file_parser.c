#include "file_parser.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "constants.h"
#include "mesh.h"

typedef struct 
{
    int vertex_count;
    int texcoord_count;
    int normal_count;
    int index_count;
} mesh_info;

static int read_line(unsigned char* src, char* dst, int cursor, int size)
{
    memset(dst, 0, size);

    int end_pos = size;
    
    /* its size - 1 because of null termination */
    for (int i = 0; i < size - 1; i++)
    {
        if (src[cursor + i] == '\n')
        {
            end_pos = i;
            break;
        }
    }

    memcpy(dst, &src[cursor], end_pos);

    return cursor + end_pos + 1;
}

static mesh_info parse_obj_mesh_info(unsigned char* buffer, int size, int cursor)
{
    mesh_info result = { 0, 0, 0, 0 };
    char line[LINE_SIZE];
    while(true)
    {

        cursor = read_line(buffer, line, cursor, LINE_SIZE);

        if (strncmp(line, "v ", 2) == 0)
            result.vertex_count++;
        else if (strncmp(line, "vt ", 3) == 0)
            result.texcoord_count++;
        else if (strncmp(line, "vn ", 3) == 0)
            result.normal_count++;
        else if (strncmp(line, "f ", 2) == 0)
            result.index_count += 3;
        
        if (cursor > size || strncmp(&buffer[cursor], "o ", 2) == 0)
            break;
    }
    return result;
}

static mesh* parse_obj_mesh(unsigned char* buffer, int size, int* cursor)
{

    mesh_info current_mesh_info = parse_obj_mesh_info(buffer, size, *cursor);

    vec* vertices = malloc(sizeof(vec) * current_mesh_info.vertex_count);
    vec* texcoords = malloc(sizeof(vec) * current_mesh_info.texcoord_count);
    vec* normals = malloc(sizeof(vec) * current_mesh_info.normal_count);
    int* vertex_indices = malloc(sizeof(int) * current_mesh_info.index_count);
    int* texcoord_indices = malloc(sizeof(int) * current_mesh_info.index_count);
    int* normal_indices = malloc(sizeof(int) * current_mesh_info.index_count);
    int vertex_index = 0;
    int texcoord_index = 0;
    int normal_index = 0;
    int vi_index = 0;
    int ti_index = 0;
    int ni_index = 0;
    
    int items;
    vec input_vec;
    char line[LINE_SIZE];
    char name[MESH_NAME_SIZE];
    while(true)
    {

        *cursor = read_line(buffer, line, *cursor, LINE_SIZE);
        if (strncmp(line, "v ", 2) == 0)
        {
            items = sscanf(line, "v %f %f %f", &input_vec.x, &input_vec.y, &input_vec.z);
            assert(items == 3);
            vertices[vertex_index++] = input_vec;
        }
        else if (strncmp(line, "vt ", 3) == 0)
        {
            items = sscanf(line, "vt %f %f", &input_vec.x, &input_vec.y);
            assert(items == 2);
            texcoords[texcoord_index++] = input_vec;
        }
        else if (strncmp(line, "vn ", 3) == 0)
        {
            items = sscanf(line, "vn %f %f %f", &input_vec.x, &input_vec.y, &input_vec.z);
            assert(items == 3);
            normals[normal_index++] = input_vec;
        }
        else if (strncmp(line, "f ", 2) == 0)
        {
            items = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                           &vertex_indices[vi_index], &texcoord_indices[ti_index], &normal_indices[ni_index],
                           &vertex_indices[vi_index + 1], &texcoord_indices[ti_index + 1], &normal_indices[ni_index + 1],
                           &vertex_indices[vi_index + 2], &texcoord_indices[ti_index + 2], &normal_indices[ni_index + 2]);
            assert(items == 9);
            vi_index += 3;
            ti_index += 3;
            ni_index += 3;
        }
        else if (strncmp(line, "o ", 2) == 0)
        {
            items = sscanf(line, "o %s", name);
            assert(items == 1);
        }

        if (*cursor > size || strncmp(&buffer[*cursor], "o ", 2) == 0)
            break;
    }

    /* assert that everything has been read */
    assert(current_mesh_info.vertex_count   == vertex_index);
    assert(current_mesh_info.texcoord_count == texcoord_index);
    assert(current_mesh_info.normal_count   == normal_index);
    assert(current_mesh_info.index_count    == vi_index);
    assert(current_mesh_info.index_count    == ti_index);
    assert(current_mesh_info.index_count    == ni_index);

    return mesh_new(name, vertices, texcoords, normals, current_mesh_info.vertex_count, 
                    current_mesh_info.texcoord_count, current_mesh_info.normal_count,
                    vertex_indices, texcoord_indices, normal_indices, current_mesh_info.index_count);
}

void parse_obj_scene(const char* file_name)
{

    /* open file */
    FILE* file = fopen(file_name, "r");
    assert(file != NULL);

    /* get file size */
    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    /* copy file contents into buffer */
    unsigned char* buffer = malloc(file_size);
    fread(buffer, sizeof(unsigned char), file_size, file);

    int cursor = 0;

    while (cursor < file_size)
    {
        if (strncmp(&buffer[cursor], "o ", 2) == 0)
        {
            mesh* mesh_data = parse_obj_mesh(buffer, file_size, &cursor);
        }
        cursor++;
    }


    fclose(file);

}