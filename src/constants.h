#pragma once

#define RGB_CHANNELS 4 /* XCreateImage fails if this is different than 1, 2 or 4 */

#define MESH_NAME_SIZE 256
#define MAX_MESHES 20 /* max number of meshes to render */

/* file parsing */
#define MAX_JSON_DEPTH          512
#define PNG_NODE_POOL_SIZE 		2 * (286 + 32 + 19)		/* LL_SIZE + D_SIZE + CL_SIZE */
/* TODO: we should not require this much space.
 * 		 Figure out how to use the sliding window
 * 		 to reduce needed memory
 */
#define PNG_BUFFER_SIZE		 	2048 * 2048 * 4 + 2048 	/* WIDTH * HEIGHT * STRIDE + FILTER_TYPE*/
#define PNG_HEADER_CHUNK 		1229472850
#define PNG_DATA_CHUNK 			1229209940
#define PNG_END_CHUNK 			1229278788
#define ZLIB_CTRL_VAL 			31
#define ZLIB_COMPRESSION 		8