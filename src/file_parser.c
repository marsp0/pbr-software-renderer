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
    uint32_t vertex_count;
    uint32_t texcoord_count;
    uint32_t normal_count;
    uint32_t index_count;
} mesh_info;

static uint32_t read_line(unsigned char* src, unsigned char* dst, uint32_t cursor, uint32_t size)
{
    memset(dst, 0, size);

    uint32_t end_pos = size;
    
    for (int i = 0; i < size - 1; i++) // its size - 1 because of null termination
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

static mesh_info parse_obj_mesh_info(unsigned char* buffer, uint32_t size, uint32_t cursor)
{
    mesh_info data = { 0, 0, 0, 0 };
    char line[LINE_SIZE];
    while(true)
    {

        cursor = read_line(buffer, line, cursor, LINE_SIZE);

        if (strncmp(line, "v ", 2) == 0)
            data.vertex_count++;
        else if (strncmp(line, "vt ", 3) == 0)
            data.texcoord_count++;
        else if (strncmp(line, "vn ", 3) == 0)
            data.normal_count++;
        else if (strncmp(line, "f ", 2) == 0)
            data.index_count += 3;
        
        if (cursor > size || strncmp(&buffer[cursor], "o ", 2) == 0)
            break;
    }
    return data;
}

static mesh* parse_obj_mesh(unsigned char* buffer, uint32_t size, uint32_t* cursor)
{

    // get sizes
    mesh_info current_mesh_info = parse_obj_mesh_info(buffer, size, *cursor);

    // parse contents
    vec* vertices = malloc(sizeof(vec) * current_mesh_info.vertex_count);
    vec* texcoords = malloc(sizeof(vec) * current_mesh_info.texcoord_count);
    vec* normals = malloc(sizeof(vec) * current_mesh_info.normal_count);
    int* vertex_indices = malloc(sizeof(int) * current_mesh_info.index_count);
    int* texcoord_indices = malloc(sizeof(int) * current_mesh_info.index_count);
    int* normal_indices = malloc(sizeof(int) * current_mesh_info.index_count);
    uint32_t vertex_index = 0;
    uint32_t texcoord_index = 0;
    uint32_t normal_index = 0;
    uint32_t vi_index = 0;
    uint32_t ti_index = 0;
    uint32_t ni_index = 0;
    
    int items;
    char line[LINE_SIZE];
    char name[MESH_NAME_SIZE];
    while(true)
    {

        *cursor = read_line(buffer, line, *cursor, LINE_SIZE);
        if (strncmp(line, "v ", 2) == 0)
        {
            vec vertex;
            items = sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            assert(items == 3);
            vertices[vertex_index] = vertex;
            vertex_index++;
        }
        else if (strncmp(line, "vt ", 3) == 0)
        {
            vec vertex;
            items = sscanf(line, "vt %f %f", &vertex.x, &vertex.y);
            assert(items == 2);
            texcoords[texcoord_index] = vertex;
            texcoord_index++;
        }
        else if (strncmp(line, "vn ", 3) == 0)
        {
            vec normal;
            items = sscanf(line, "vn %f %f %f", &normal.x, &normal.y, &normal.z);
            assert(items == 3);
            normals[normal_index] = normal;
            normal_index++;
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

    // assert that everything has been read
    assert(current_mesh_info.vertex_count == vertex_index);
    assert(current_mesh_info.texcoord_count == texcoord_index);
    assert(current_mesh_info.normal_count == normal_index);
    assert(current_mesh_info.index_count == vi_index);
    assert(current_mesh_info.index_count == ti_index);
    assert(current_mesh_info.index_count == ni_index);

    return mesh_new(name, vertices, texcoords, normals, current_mesh_info.vertex_count, 
                    current_mesh_info.texcoord_count, current_mesh_info.normal_count,
                    vertex_indices, texcoord_indices, normal_indices, current_mesh_info.index_count);
}

void parse_obj_scene(const char* file_name)
{

    // open file
    FILE* file = fopen(file_name, "r");
    assert(file != NULL);

    // get file size
    fseek(file, 0, SEEK_END);
    uint32_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // copy file contents into buffer
    unsigned char* buffer = malloc(file_size);
    fread(buffer, sizeof(unsigned char), file_size, file);

    uint32_t cursor = 0;

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