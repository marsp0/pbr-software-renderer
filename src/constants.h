#pragma once

#define RGB_CHANNELS 4 /* XCreateImage fails if this is different than 1, 2 or 4 */

#define MESH_NAME_SIZE 256
#define MAX_MESHES 20 /* max number of meshes to render */

/* file parsing */
#define MAX_JSON_DEPTH          512