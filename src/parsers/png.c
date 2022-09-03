#include "png.h"

#include <stdint.h>
#include <assert.h>
#include <stdio.h>

/*
 * List of resources that helped with the parsing of png
 * - PNG spec - http://www.libpng.org/pub/png/spec/1.2/PNG-Contents.html
 * - ZLIB format spec - https://www.rfc-editor.org/rfc/rfc1950.html
 * - Deflate spec - https://www.rfc-editor.org/rfc/rfc1951.html
 * - Deflate blog post - https://zlib.net/feldspar.html
 * - Deflate block format (for noobies like me) - https://stackoverflow.com/questions/32419086/the-structure-of-deflate-compressed-block 
 */

/*
 * Huffman
 */




/*
 * PNG
 */

typedef struct
{
    uint32_t    width;
    uint32_t    height;
    uint8_t     bits_per_pixel;
    uint8_t     color_type;
    uint8_t     compression;
    uint8_t     filter;
    uint8_t     interlace;
} header_t;

typedef struct
{
    uint32_t        size;
    uint32_t        type;
    uint32_t        crc;
    unsigned char*  data;
} chunk_t;


const int DEFLATE_COMPRESSION = 0x8;
const int HEADER_CTRL_VAR = 31;
static int cursor = 0;


static chunk_t parse_chunk(unsigned char* buffer);
static header_t parse_header(unsigned char* buffer);
static uint32_t parse_int(unsigned char* buffer, int n);
static void assert_png(unsigned char* buffer);
static void assert_zlib(unsigned char* buffer);

void parse_png(unsigned char* buffer, size_t size)
{
    assert_png(buffer);

    header_t header = parse_header(buffer);
    chunk_t chunk = parse_chunk(buffer);

    assert_zlib(chunk.data);
    printf("cursor %d\n", cursor);
    printf("chunk.size: %d\n", chunk.size);


    printf("%x\n", chunk.data[2]);
    printf("%x\n", chunk.data[3]);
    printf("%x\n", chunk.data[4]);
    printf("%x\n", chunk.data[5]);
    printf("%x\n", chunk.data[6]);

}

static header_t parse_header(unsigned char* buffer)
{
    uint32_t size = parse_int(buffer, 4);
    uint32_t type = parse_int(buffer, 4);

    /* size of header and type */
    assert(size == 13);
    assert(type == 1229472850); /* IHDR */

    header_t header;
    header.width = parse_int(buffer, 4);
    header.height = parse_int(buffer, 4);
    header.bits_per_pixel = (uint8_t)parse_int(buffer, 1);
    header.color_type = (uint8_t)parse_int(buffer, 1);
    header.compression = (uint8_t)parse_int(buffer, 1);
    header.filter = (uint8_t)parse_int(buffer, 1);
    header.interlace = (uint8_t)parse_int(buffer, 1);

    cursor += 4; /* crc bytes */

    return header;
}

static chunk_t parse_chunk(unsigned char* buffer)
{
    chunk_t chunk;
    chunk.size = parse_int(buffer, 4);
    chunk.type = parse_int(buffer, 4);
    chunk.data = &buffer[cursor];

    cursor += chunk.size; /* data size */

    chunk.crc = parse_int(buffer, 4);

    /*IDAT or IEND*/
    assert(chunk.type == 1229209940 || chunk.type == 1229278788);

    return chunk;
}

static uint32_t parse_int(unsigned char* buffer, int n)
{
    uint32_t result = 0;
    for (int i = cursor; i < cursor + n; i++)
    {
        result += buffer[i] << ((n - (i - cursor) - 1) * 8);
    }
    cursor += n;
    return result;
}

static void assert_png(unsigned char* buffer)
{
    assert(buffer[0] == 137);
    assert(buffer[1] == 80);
    assert(buffer[2] == 78);
    assert(buffer[3] == 71);
    assert(buffer[4] == 13);
    assert(buffer[5] == 10);
    assert(buffer[6] == 26);
    assert(buffer[7] == 10);

    cursor = 8;
}

static void assert_zlib(unsigned char* buffer)
{
    assert(buffer[0] & DEFLATE_COMPRESSION);
    uint16_t tmp = (buffer[0] << 8) + buffer[1];
    assert(tmp % HEADER_CTRL_VAR == 0);
}