#include "renderer.h"

#include <stddef.h>

renderer_t* renderer_new(int width, int height, const char* file_name)
{

    renderer_t* renderer = malloc(sizeof(renderer_t));

    /*renderer->meshes_size = parse_obj_scene(file_name, renderer->meshes, MESHES_CAPACITY);*/
    /* 
     * TODO 
     * allocate framebuffer
     * allocate depthbuffer
     */
    return renderer;
}

void renderer_run(renderer_t* renderer)
{

}

void renderer_render(renderer_t* renderer)
{

}

void renderer_render_mesh(renderer_t* renderer)
{

}

void renderer_clear(renderer_t* renderer)
{

}

void renderer_free(renderer_t* renderer)
{
    /*free framebuffer*/
    /*free deothbuffer*/
    free(renderer);
}