#include "png.h"

#include <stdint.h>
#include <assert.h>
#include <stdio.h>

#include "../constants.h"

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
    const unsigned char*  data;
} chunk_t;

#define PNG_CHUNK_CAPACITY 700

const uint32_t ZLIB_DEFLATE_COMPRESSION  = 0x8;
const uint32_t ZLIB_HEADER_CONTROL_VAL   = 31;
const uint32_t PNG_HEADER_CHUNK          = 1229472850;
const uint32_t PNG_DATA_CHUNK            = 1229209940;
const uint32_t PNG_END_CHUNK             = 1229278788;

static uint8_t  bit_buffer   = 0;
static uint8_t  bit_count    = 0;
static uint32_t cursor       = 0;
static uint32_t chunks_index = 0;
static chunk_t chunks[PNG_CHUNK_CAPACITY];

static void     parse_chunk(const unsigned char* buffer);
static header_t parse_header(const unsigned char* buffer);
static uint32_t parse_int(const unsigned char* buffer, int n);
static void     assert_png(const unsigned char* buffer);
static void     assert_zlib(const unsigned char* buffer);
static uint32_t parse_bits(const unsigned char* buffer, int n);

void parse_png(const unsigned char* buffer, size_t size)
{
    /* assert that buffer is PNG*/
    assert(buffer[0] == 137);
    assert(buffer[1] == 80);
    assert(buffer[2] == 78);
    assert(buffer[3] == 71);
    assert(buffer[4] == 13);
    assert(buffer[5] == 10);
    assert(buffer[6] == 26);
    assert(buffer[7] == 10);

    cursor = 8;
    chunks_index = 0;

    header_t header = parse_header(buffer);
    do
    {
        parse_chunk(buffer);
    } while (chunks[chunks_index - 1].type != PNG_END_CHUNK);

    /* assert that first chunk has zlib header */
    assert(chunks[0].data[0] & ZLIB_DEFLATE_COMPRESSION);
    assert(((uint16_t)(chunks[0].data[0] << 8) + (uint16_t)chunks[0].data[1]) % ZLIB_HEADER_CONTROL_VAL == 0);
    
    /* at this point cursor will be used to index chunks data and not png buffer */
    cursor = 2;
    int last = 1;
    do
    {
        /*last = parse_bits()*/
    } while (!last);
}

static header_t parse_header(const unsigned char* buffer)
{
    uint32_t size = parse_int(buffer, 4);
    uint32_t type = parse_int(buffer, 4);

    assert(size == 13);
    assert(type == PNG_HEADER_CHUNK);

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

static void parse_chunk(const unsigned char* buffer)
{
    chunks[chunks_index].size = parse_int(buffer, 4);
    chunks[chunks_index].type = parse_int(buffer, 4);
    chunks[chunks_index].data = &buffer[cursor];

    cursor += chunks[chunks_index].size;

    chunks[chunks_index].crc = parse_int(buffer, 4);

    assert(chunks[chunks_index].type == PNG_DATA_CHUNK || 
           chunks[chunks_index].type == PNG_END_CHUNK);

    chunks_index++;
    assert(chunks_index < PNG_CHUNK_CAPACITY);
}

static uint32_t parse_int(const unsigned char* buffer, int n)
{
    uint32_t result = 0;
    for (int i = cursor; i < cursor + n; i++)
    {
        result += buffer[i] << ((n - (i - cursor) - 1) * 8);
    }
    cursor += n;
    return result;
}

/*
 * parse_bits - parses up to 32 bits from the stream.
 * we read 8 bits at a time until we've read at least n bits.
 * Since we are reading multiples of 8 bits but might not actually need them all, we need a place to store the 
 * remaining bits for the next parse (bit_buffer, bit_count)
 */
static uint32_t parse_bits(const unsigned char* buffer, int n)
{
    assert(n <= 32);

    uint32_t result = bit_buffer;
    while (bit_count < n)
    {
        result += buffer[cursor] << bit_count;

        bit_count += 8;
        bit_buffer = buffer[cursor];
        cursor += 1;
    }

    bit_buffer = result >> n;
    bit_count -= n;

    /* remove bits that were extra */
    result &= (1L << n) - 1;

    return result;
}