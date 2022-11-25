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
    uint16_t codes[PNG_ALPHABET_CAPACITY];
} alphabet_t;

/*temporary*/
static unsigned char result[50] = { 0 };
static uint32_t result_index = 0;

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
static uint32_t parse_int(const unsigned char* buffer, int n)
{
    assert(n <= 4);

    uint32_t result = 0;
    for (uint32_t i = cursor; i < cursor + n; i++)
        result += buffer[i] << ((n - (i - cursor) - 1) * 8);

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

static uint32_t parse_symbol(const unsigned char* buffer, alphabet_t* alphabet)
{
    uint32_t result = 0;
    uint32_t code = 0;
    uint32_t code_len = 0;

    while (true)
    {
        /* parse code (MSB) */
        code = (code << 1) + parse_bits(buffer, 1);
        code_len++;
        
        for (int i = 0; i < alphabet->size; i++)
        {
            if (alphabet->lengths[i] == code_len && alphabet->codes[i] == code)
                return i;
        }

        if (result == -1)
            continue;
    }

    return result;
}

static void decode(const unsigned char* buffer)
{
    uint32_t ll_symbol = 0;
    while (true)
    {
        ll_symbol = parse_symbol(buffer, &ll_alphabet);

        if (ll_symbol <= 255)                                                   /* the symbol is a literal */
        {
            result[result_index] = ll_symbol;
            result_index++;
            printf("index: %d, value: %d\n", result_index, ll_symbol);
        }
        else if (ll_symbol == 256)                                              /* end of block symbol */
            break;

        else                                                                    /* symbol is a length (followed by distance) */
        {
            uint32_t ll_index = ll_symbol - 257;                                /* in ll_map 0 maps to 257, 1 maps to 258 etc. */
            uint32_t len = ll_map[ll_index][1] + parse_bits(buffer, ll_map[ll_index][0]);

            uint32_t d_symbol = parse_symbol(buffer, &d_alphabet);
            uint32_t distance = d_map[d_symbol][1] + parse_bits(buffer, d_map[d_symbol][0]);

            int start = result_index - distance;
            for (int i = start; i < start + len; i++)
            {
                result[result_index] = result[i];
                result_index++;
            }
        }
    }

    for (int i = 0; i < 50; i++)
    {
        printf("%c", result[i]);
    }
    printf("\n");
}

static void generate_huffman_codes(alphabet_t* alphabet)
{
    /* count alphabet lens */
    uint8_t len_counts[15] = {0};
    uint8_t base_vals[15] = {0};

    for (int i = 0; i < alphabet->size; i++)
        len_counts[alphabet->lengths[i]]++;

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
            continue;

        alphabet->codes[i] = base_vals[code_len];
        base_vals[code_len]++;
    }
}

static void parse_alphabet(const unsigned char* buffer, alphabet_t* alphabet)
{
    uint32_t symbol = 0;
    uint32_t index = 0;

    while (index < alphabet->size)
    {
        symbol = parse_symbol(buffer, &cl_alphabet);

        if (symbol <= 15)                                               /* symbol is literal */
        {
            alphabet->lengths[index] = symbol;
            index += 1;
        }
        else if (symbol == 16)                                          /* repeat last symbol x times */
        {
            uint32_t count = parse_bits(buffer, 2) + 3;
            
            for (int i = index; i < (index + count); i++)
                alphabet->lengths[i] = alphabet->lengths[index - 1];

            index += count;
        }
        else if (symbol == 17)                                          /* repeat zero x times */
            index += parse_bits(buffer, 3) + 3;

        else                                                            /* repeat zero y times */
            index += parse_bits(buffer, 7) + 11;
    }

    generate_huffman_codes(alphabet);
}

static void parse_cl_alphabet(const unsigned char* buffer, int cl_size)
{
    cl_alphabet.size = 19;
    const uint8_t order[] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };

    for (int i = 0; i < cl_size; i++)
        cl_alphabet.lengths[order[i]] = parse_bits(buffer, 3);

    generate_huffman_codes(&cl_alphabet);
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

    /* first two bytes are garbage, deflate starts after */
    const unsigned char buf[] = {0x78, 0xDA, 0x1d, 0xc6, 0x49, 0x01, 0x00, 0x00, 0x10, 0x40, 0xc0, 0xac, 0xa3, 0x7f, 0x88, 
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

        ll_alphabet.size = parse_bits(buf, 5) + 257;
        d_alphabet.size = parse_bits(buf, 5) + 1;
        uint32_t cl_size = parse_bits(buf, 4) + 4;

        parse_cl_alphabet(buf, cl_size);
        parse_alphabet(buf, &ll_alphabet);
        parse_alphabet(buf, &d_alphabet);
        decode(buf);

    } while (!last);
}
