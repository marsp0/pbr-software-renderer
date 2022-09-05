#pragma once

#define RGB_CHANNELS 4 /* XCreateImage fails if this is different than 1, 2 or 4 */

#define MESH_NAME_SIZE 256
#define MESHES_CAPACITY 20 /* max number of meshes to render */

/* file parsing defines */
#define JSON_TOKENS_CAPACITY 1000
#define PNG_CHUNK_CAPACITY 700