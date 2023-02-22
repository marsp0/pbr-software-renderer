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

/* glb json keys */
#define JSON_SCENE              "scene"
#define JSON_SCENES             "scenes"
#define JSON_ASSET              "asset"
#define JSON_VERSION            "version"
#define JSON_ACCESSORS          "accessors"
#define JSON_BUFFER_VIEWS       "bufferViews"
#define JSON_BUFFERS            "buffers"
#define JSON_IMAGES             "images"
#define JSON_MESHES             "meshes"
#define JSON_MATERIALS          "materials"
#define JSON_NODES              "nodes"
#define JSON_TEXTURES           "textures"
#define JSON_BUFFER_VIEW        "bufferView"
#define JSON_COMP_TYPE          "componentType"
#define JSON_COUNT              "count"
#define JSON_TYPE               "type"
#define JSON_BUFFER             "buffer"
#define JSON_BYTE_LEN           "byteLength"
#define JSON_BYTE_OFFSET        "byteOffset"
#define JSON_MIME_TYPE          "mimeType"
#define JSON_PRIMITIVES         "primitives"
#define JSON_NAME               "name"
#define JSON_ATTRIBUTES         "attributes"
#define JSON_INDICES            "indices"
#define JSON_MATERIAL           "material"
#define JSON_TEXCOORD_0         "TEXCOORD_0"
#define JSON_NORMAL             "NORMAL"
#define JSON_TANGENT            "TANGENT"
#define JSON_POSITION           "POSITION"
#define JSON_MET_ROUGHNESS      "pbrMetallicRoughness"
#define JSON_BASE_COL_TEX       "baseColorTexture"
#define JSON_INDEX              "index"
#define JSON_MET_ROUGH_TEX      "metallicRoughnessTexture"
#define JSON_NORMAL_TEX         "normalTexture"
#define JSON_OCCLUSION_TEX      "occlusionTexture"
#define JSON_EMISSIVE_FACTOR    "emissiveFactor"
#define JSON_EMISSIVE_TEX       "emissiveTexture"
#define JSON_MESH               "mesh"
#define JSON_ROTATION           "rotation"
#define JSON_NODES              "nodes"
#define JSON_SOURCE             "source"