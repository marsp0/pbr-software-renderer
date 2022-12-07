#pragma once

#define RGB_CHANNELS 4 /* XCreateImage fails if this is different than 1, 2 or 4 */

#define MESH_NAME_SIZE 256
#define MESHES_CAPACITY 20 /* max number of meshes to render */

/* file parsing */
#define JSON_TOKENS_CAPACITY 1000
#define PNG_CHUNK_CAPACITY 700
#define PNG_ALPHABET_CAPACITY 287
#define PNG_NODE_POOL_SIZE 2 * (286 + 32 + 19)		/* LL_SIZE + D_SIZE + CL_SIZE */
#define PNG_MAX_BUFFER_SIZE 2048 * 2048 * 4 + 2048 	/* WIDTH * HEIGHT * STRIDE + FILTER_TYPE*/