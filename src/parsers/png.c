#include "png.h"

#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../constants.h"
#include "../texture.h"

/*
 * List of resources that helped with the parsing of png
 * - PNG spec - http://www.libpng.org/pub/png/spec/1.2/PNG-Contents.html
 * - ZLIB format spec - https://www.rfc-editor.org/rfc/rfc1950.html
 * - Deflate spec - https://www.rfc-editor.org/rfc/rfc1951.html
 * - Deflate blog post - https://zlib.net/feldspar.html
 * - GZIP post containing deflate information
 *      - www.infinitepartitions.com/art001.html
 * - gzip parser that i used to validate my code
 *      - https://github.com/billbird/gzstat
 * - So questions - 
 *      https://stackoverflow.com/questions/32419086/the-structure-of-deflate-compressed-block 
 *      https://stackoverflow.com/questions/53108124/understanding-idat-reading-deflates-dynamic-huffman-tree
 *      https://stackoverflow.com/questions/72325918/deflate-block-header
 *      https://stackoverflow.com/questions/72144290/puff-c-how-does-the-huffman-decoding-work
 *      https://stackoverflow.com/questions/27713498/examples-of-deflate-compression
 */

/* TODO: 
 * 1. describe what is happening here 
 * 2. rather than passing buffer around, just assign it to a static variable
 * 3. assign texture pointer to a static variable for easy access
*/

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
    uint32_t end;
} chunk_t;

typedef struct
{
    uint16_t size;
    uint16_t lengths[PNG_ALPHABET_CAPACITY];
    uint16_t codes[PNG_ALPHABET_CAPACITY];
} alphabet_t;

const uint32_t ZLIB_COMPRESSION = 8;
const uint32_t ZLIB_CTRL_VAL    = 31;
const uint32_t PNG_HEADER_CHUNK = 1229472850;
const uint32_t PNG_DATA_CHUNK   = 1229209940;
const uint32_t PNG_END_CHUNK    = 1229278788;

static uint8_t  bit_buffer              = 0;
static uint8_t  bit_count               = 0;
static uint32_t src_cursor              = 0;
static uint32_t src_buffer_size         = 0;
static const unsigned char* src_buffer  = NULL;

static uint32_t dst_cursor              = 0;
static const unsigned char* dst_buffer  = 0;

static texture_t* texture           = NULL;
static uint32_t tex_cursor          = 0;

static header_t header          = { 0 };
static chunk_t chunk            = { 0 };

static alphabet_t cl_alphabet   = { 0 };
static alphabet_t ll_alphabet   = { 0 };
static alphabet_t d_alphabet    = { 0 };
static uint32_t ll_map[29][2]   = {
    {0, 3},    {0, 4},    {0, 5},    {0, 6},    {0, 7}, 
    {0, 8},    {0, 9},    {0, 10},   {1, 11},   {1, 13},
    {1, 15},   {1, 17},   {2, 19},   {2, 23},   {2, 27},
    {2, 31},   {3, 35},   {3, 43},   {3, 51},   {3, 59},
    {4, 67},   {4, 83},   {4, 99},   {4, 115},  {5, 131},
    {5, 163},  {5, 195},  {5, 227},  {0, 258}
};
static uint32_t d_map[30][2] = {
    {0, 1},     {0, 2},     {0, 3},     {0, 4},     {1, 5},
    {1, 7},     {2, 9},     {2, 13},    {3, 17},    {3, 25},
    {4, 33},    {4, 49},    {5, 65},    {5, 97},    {6, 129},
    {6, 193},   {7, 257},   {7, 385},   {8, 513},   {8, 769},
    {9, 1025},  {9, 1537},  {10, 2049}, {10, 3073}, {11, 4097},
    {11, 6145}, {12, 8193}, {12, 12289},{13, 16385},{13, 24577},
};

/*
 * parse_int - parses up to 4 bytes from the stream. MSB
 */
static uint32_t parse_int(int n)
{
    assert(n <= 4);

    uint32_t result = 0;
    for (uint32_t i = src_cursor; i < src_cursor + n; i++)
        result += src_buffer[i] << ((n - (i - src_cursor) - 1) * 8);

    src_cursor += n;
    return result;
}

static void parse_chunk();
/*
 * parse_bits - parses up to 32 bits from the stream.
 * we read 8 bits at a time until we've read at least n bits.
 * Since we are reading multiples of 8 bits but might not actually need them all, we need a place to store the 
 * remaining bits for the next parse (bit_buffer, bit_count)
 */
static uint32_t parse_bits(int n)
{
    assert(n <= 32);

    uint32_t result = bit_buffer;
    while (bit_count < n)
    {
        result += src_buffer[src_cursor] << bit_count;

        bit_count += 8;
        bit_buffer = src_buffer[src_cursor];
        src_cursor += 1;

        if (src_cursor == chunk.end)
        {
            parse_chunk();
        }
    }

    bit_buffer = result >> n;
    bit_count -= n;

    /* remove bits that were extra */
    result &= (1L << n) - 1;

    return result;
}

static uint32_t parse_symbol(alphabet_t* alphabet)
{
    uint32_t code = 0;
    uint32_t code_len = 0;

    while (true)
    {
        /* parse code (MSB) */
        code = (code << 1) + parse_bits(1);
        code_len++;
        
        for (uint32_t i = 0; i < alphabet->size; i++)
        {
            if (alphabet->lengths[i] == code_len && alphabet->codes[i] == code)
            {
                return i;
            }
        }
    }
    assert(false);
}

static void decode()
{
    uint32_t ll_symbol = 0;
    while (ll_symbol != 256)
    {
        ll_symbol = parse_symbol(&ll_alphabet);

        if (ll_symbol <= 255)                                                   /* the symbol is a literal */
        {
            texture->data[tex_cursor] = ll_symbol;
            tex_cursor++;
        }
        else if (ll_symbol > 256)                                               /* symbol is a length (followed by distance) */
        {
            uint32_t ll_index = ll_symbol - 257;                                /* in ll_map 0 maps to 257, 1 maps to 258 etc. */
            uint32_t len = ll_map[ll_index][1] + parse_bits(ll_map[ll_index][0]);

            uint32_t d_symbol = parse_symbol(&d_alphabet);
            uint32_t distance = d_map[d_symbol][1] + parse_bits(d_map[d_symbol][0]);

            memcpy(&(texture->data[tex_cursor]), &(texture->data[tex_cursor - distance]), len);
            tex_cursor += len;
        }
        assert(tex_cursor <= (texture->width * texture->height * texture->stride));
    }
}

static void generate_huffman_codes(alphabet_t* alphabet)
{
    /* count alphabet lens */
    uint32_t len_counts[15] = {0};
    uint32_t base_vals[15] = {0};

    for (int i = 0; i < alphabet->size; i++)
    {
        len_counts[alphabet->lengths[i]]++;
    }

    /* find base code for each length */
    int code = 0;
    len_counts[0] = 0;
    for (int i = 1; i < 16; i++)
    {
        code = (code + len_counts[i - 1]) << 1;
        base_vals[i] = code;
    }

    /* give each code a value based on the base value */
    for (int i = 0; i < alphabet->size; i++)
    {
        int code_len = alphabet->lengths[i];
        if (code_len == 0)
        {
            continue;
        }

        alphabet->codes[i] = base_vals[code_len];
        base_vals[code_len]++;
    }
}

static void parse_alphabet(alphabet_t* alphabet)
{
    uint32_t symbol = 0;
    uint32_t index = 0;

    while (index < alphabet->size)
    {
        symbol = parse_symbol(&cl_alphabet);

        if (symbol <= 15)                                               /* symbol is literal */
        {
            alphabet->lengths[index] = symbol;
            index += 1;
        }
        else if (symbol == 16)                                          /* repeat last symbol x times */
        {
            uint32_t count = parse_bits(2) + 3;
            
            for (int i = index; i < (index + count); i++)
                alphabet->lengths[i] = alphabet->lengths[index - 1];

            index += count;
        }
        else if (symbol == 17)                                          /* repeat zero x times */
        {
            index += parse_bits(3) + 3;
        }
        else                                                            /* repeat zero y times */
        {
            index += parse_bits(7) + 11;
        }
    }

    generate_huffman_codes(alphabet);
}

static void parse_cl_alphabet(int cl_size)
{
    cl_alphabet.size = 19;
    const uint8_t order[] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };

    for (int i = 0; i < cl_size; i++)
    {
        cl_alphabet.lengths[order[i]] = parse_bits(3);
    }

    generate_huffman_codes(&cl_alphabet);
}

static void parse_chunk()
{
    src_cursor += 4; /* crc bytes from previous chunk */
    chunk.size = parse_int(4);
    chunk.type = parse_int(4);
    chunk.end = src_cursor + chunk.size;
}

static void parse_header()
{
    uint32_t size = parse_int(4);
    uint32_t type = parse_int(4);

    assert(size == 13);
    assert(type == PNG_HEADER_CHUNK);

    header.width = parse_int(4);
    header.height = parse_int(4);
    header.bits_per_pixel = (uint8_t)parse_int(1);
    header.color_type = (uint8_t)parse_int(1);
    header.compression = (uint8_t)parse_int(1);
    header.filter = (uint8_t)parse_int(1);
    header.interlace = (uint8_t)parse_int(1);

    assert(header.bits_per_pixel == 8);
    assert(header.color_type == 2 || header.color_type == 6);
}

static void parse_deflate_stream()
{
    assert(src_buffer[src_cursor] & ZLIB_COMPRESSION);
    assert(((src_buffer[src_cursor] << 8) + \
            src_buffer[src_cursor + 1]) % ZLIB_CTRL_VAL == 0);
    
    src_cursor += 2;

    int last = 0;
    int type = 0;
    do
    {
        memset(&cl_alphabet, 0, sizeof(cl_alphabet));
        memset(&ll_alphabet, 0, sizeof(ll_alphabet));
        memset(&d_alphabet, 0, sizeof(d_alphabet));

        last = parse_bits(1);
        type = parse_bits(2);

        if (type != 2)
        {
            printf("Huffman code type(%d) - %d not supported\n", type, last);
            exit(1);
        }

        ll_alphabet.size = parse_bits(5) + 257;
        d_alphabet.size = parse_bits(5) + 1;
        uint32_t cl_size = parse_bits(4) + 4;

        parse_cl_alphabet(cl_size);
        parse_alphabet(&ll_alphabet);
        parse_alphabet(&d_alphabet);
        decode();

    } while (!last);
}

void parse_png(const unsigned char* buf, size_t size)
{
    /* set up static vars */
    src_buffer = buf;
    src_buffer_size = size;

    /* assert that buffer is PNG*/
    assert(src_buffer[0] == 137);
    assert(src_buffer[1] == 80);
    assert(src_buffer[2] == 78);
    assert(src_buffer[3] == 71);
    assert(src_buffer[4] == 13);
    assert(src_buffer[5] == 10);
    assert(src_buffer[6] == 26);
    assert(src_buffer[7] == 10);
    src_cursor = 8;

    parse_header();

    int stride = header.color_type == 2 ? 3 : 4;                    /* RGB or RGBA */
    texture = texture_new(header.width, header.height, stride);
    tex_cursor = 0;

    while (chunk.type != PNG_END_CHUNK)
    {
        parse_chunk();

        if (chunk.type == PNG_DATA_CHUNK)
        {
            parse_deflate_stream();
        }

        src_cursor = chunk.end;
    } 
}
