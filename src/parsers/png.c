#include "png.h"

#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../constants.h"

/*
 * List of resources that helped with the parsing of png
 * - PNG spec - http://www.libpng.org/pub/png/spec/1.2/PNG-Contents.html
 * - ZLIB format spec - https://www.rfc-editor.org/rfc/rfc1950.html
 * - Deflate spec - https://www.rfc-editor.org/rfc/rfc1951.html
 * - Deflate blog post - https://zlib.net/feldspar.html
 * - GZIP post containing deflate information
 *      - www.infinitepartitions.com/art001.html
 * - So questions - 
 *      https://stackoverflow.com/questions/32419086/the-structure-of-deflate-compressed-block 
 *      https://stackoverflow.com/questions/53108124/understanding-idat-reading-deflates-dynamic-huffman-tree
 *      https://stackoverflow.com/questions/72325918/deflate-block-header
 *      https://stackoverflow.com/questions/72144290/puff-c-how-does-the-huffman-decoding-work
 *      https://stackoverflow.com/questions/27713498/examples-of-deflate-compression
 */

/* TODO: describe what is happening here */

typedef struct
{
    uint32_t width;
    uint32_t height;
    uint8_t  bits_per_pixel;
    uint8_t  color_type;
    uint8_t  compression;
    uint8_t  filter;
    uint8_t  interlace;
} header_t;

typedef struct
{
    uint32_t size;
    uint32_t type;
    uint32_t crc;
    const unsigned char* data;
} chunk_t;

typedef struct
{
    uint16_t size;
    uint16_t lengths[PNG_ALPHABET_CAPACITY];
    uint16_t values[PNG_ALPHABET_CAPACITY];
} alphabet_t;

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
static alphabet_t ll_alphabet;
static alphabet_t d_alphabet;

static void     parse_chunk(const unsigned char* buffer);
static header_t parse_header(const unsigned char* buffer);
static uint32_t parse_int(const unsigned char* buffer, int n);
static void     parse_alphabets(const unsigned char* buffer, int cl_size, int ll_size, int d_size);
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
    int last = 0;
    int type = 0;
    /*const unsigned char buf[] = {0x78, 0xDA, 0xED, 0xCF, 0x31, 0x11, 0x08, 0xA1, 0xEF, 0x5F, 0x5A, 0x33, 0xB8, 
                                 0x7A, 0x0C, 0x04, 0x20, 0xA9, 0xF9, 0x20, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
                                 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
                                 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
                                 0x11, 0x11, 0x11, 0x11, 0x11, 0x91, 0x8B, 0x05, 0xB0, 0x33, 0x75, 0x96, 0x79, 
                                 0xC5, 0x1C, 0xB1};*/

    const unsigned char buf[] = {0x1d, 0xc6, 0x49, 0x01, 0x00, 0x00, 0x10, 0x40, 0xc0, 0xac, 0xa3, 0x7f, 0x88, 
                                 0x3d, 0x3c, 0x20, 0x2a, 0x97, 0x9d, 0x37, 0x5e, 0x1d, 0x0c};

    do
    {
        last = parse_bits(buf, 1);
        type = parse_bits(buf, 2);
        if (type != 0x2)
        {
            printf("Huffman code type(%d) not supported\n", type);
            exit(1);
        }

        uint32_t ll_size = parse_bits(buf, 5) + 257;
        uint32_t d_size = parse_bits(buf, 5) + 1;
        uint32_t cl_size = parse_bits(buf, 4) + 4;

        parse_alphabets(buf, cl_size, ll_size, d_size);
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

static void parse_alphabets(const unsigned char* buffer, int cl_size, int ll_size, int d_size)
{
    /* init alphabets */
    ll_alphabet.size = ll_size;
    d_alphabet.size = d_size;
    for (uint32_t i = 0; i < PNG_ALPHABET_CAPACITY; i++)
    {
        ll_alphabet.lengths[i] = 0;
        ll_alphabet.values[i] = 0;

        d_alphabet.lengths[i] = 0;
        d_alphabet.values[i] = 0;
    }

    /* parse + construct huffman codes for code length alphabet */
    uint16_t cl_order[] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
    uint8_t cl_lens[19] = {0};
    uint8_t cl_len_count[8] = {0};
    uint8_t cl_base_vals[8] = {0};
    uint8_t cl_vals[19] = {0};

    /* parse cl alphabet lens */
    for (uint32_t i = 0; i < cl_size; i++)
        cl_lens[cl_order[i]] = parse_bits(buffer, 3);

    /* count lengths */
    for (uint32_t i = 0; i < 19; i++)
        cl_len_count[cl_lens[i]]++;

    /* get base value for each length */
    int code = 0;
    cl_len_count[0] = 0;
    for (uint32_t i = 1; i < 8; i++)
    {
        code = (code + cl_len_count[i - 1]) << 1;
        cl_base_vals[i] = code;
    }

    for (uint32_t i = 0; i < 19; i++)
    {
        int len = cl_lens[i];
        if (len == 0)
            continue;
        cl_vals[i] = cl_base_vals[len];
        cl_base_vals[len]++;
    }

    int codes_read = 0;
    int bits_read = 0;
    bool found = false;
    uint32_t symbol = 0;

    for (int i = 0; i < 19; i++)
    {
        printf("%d,\t", cl_vals[i]);
    }
    printf("\n");

    /* parse both ll and d alphabets */
    while (codes_read < ll_size + d_size)
    {
        uint32_t bits = parse_bits(buffer, 1) << bits_read;
        symbol += bits;
        bits_read += 1;
        printf("bits: %d\n", bits);
        for (int i = 0; i < 19; i++)
        {
            if (cl_lens[i] == bits_read && cl_vals[i] == symbol)
            {
                found = true;
                printf("Found match at %d (%d, %d)\n", i, bits_read, cl_lens[i]);
                break;
            }
        }

        codes_read++;
        if (!found)
            continue;

        if (symbol <= 15)
        {
            printf("%d\n", symbol);
        }
        else if (symbol == 16)
        {
            printf("16\n");
        }
        else if (symbol == 17)
        {
            printf("17\n");
        }
        else
        {
            printf("18\n");
        }

        codes_read++;
        found = false;
        symbol = 0;
        bits_read = 0;
    }
}

static uint32_t parse_int(const unsigned char* buffer, int n)
{
    uint32_t result = 0;
    for (uint32_t i = cursor; i < cursor + n; i++)
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