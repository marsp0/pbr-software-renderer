#pragma once

#define MESH_NAME_SIZE 256
#define LINE_SIZE 256
#define RGB_CHANNELS 4 /* XCreateImage fails if this is different than 1, 2 or 4 */
#define MESHES_CAPACITY 20 /* max number of meshes to render */
#define JSON_TOKENS_CAPACITY 1000