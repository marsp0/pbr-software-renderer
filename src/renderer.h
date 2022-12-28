#pragma once

#include "frame_buffer.h"
#include "depth_buffer.h"
#include "mesh.h"
#include "constants.h"

/*TODO: header files should not include other header files*/

typedef struct
{
    int             width;
    int             height;
    mesh_t*         mesh;
    frame_buffer_t* frame_buffer;
    depth_buffer_t* depth_buffer;
} renderer_t;

renderer_t* renderer_new(int width, int height, const char* file_name);
void renderer_run(renderer_t* renderer);
void renderer_render(renderer_t* renderer);
void renderer_render_mesh(renderer_t* renderer);
void renderer_clear(renderer_t* renderer);
void renderer_free(renderer_t* renderer);
