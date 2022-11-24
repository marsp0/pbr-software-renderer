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
static alphabet_t cl_alphabet = { 0 };
static alphabet_t ll_alphabet = { 0 };
static alphabet_t d_alphabet = { 0 };
static uint32_t ll_map[29][3] = {
    {257, 0, 3},    {258, 0, 4},    {259, 0, 5},    {260, 0, 6},    {261, 0, 7}, 
    {262, 0, 8},    {263, 0, 9},    {264, 0, 10},   {265, 1, 11},   {266, 1, 13},
    {267, 1, 15},   {268, 1, 17},   {269, 2, 19},   {270, 2, 23},   {271, 2, 27},
    {272, 2, 31},   {273, 3, 35},   {274, 3, 43},   {275, 3, 51},   {276, 3, 59},
    {277, 4, 67},   {278, 4, 83},   {279, 4, 99},   {280, 4, 115},  {281, 5, 131},
    {282, 5, 163},  {283, 5, 195},  {284, 5, 227},  {285, 0, 258}
};
static uint32_t d_map[30][3] = {
    {0, 0, 1},      {1, 0, 2},      {2, 0, 3},      {3, 0, 4},      {4, 1, 5},
    {5, 1, 7},      {6, 2, 9},      {7, 2, 13},     {8, 3, 17},     {9, 3, 25},
    {10, 4, 33},    {11, 4, 49},    {12, 5, 65},    {13, 5, 97},    {14, 6, 129},
    {15, 6, 193},   {16, 7, 257},   {17, 7, 385},   {18, 8, 513},   {19, 8, 769},
    {20, 9, 1025},  {21, 9, 1537},  {22, 10, 2049}, {23, 10, 3073}, {24, 11, 4097},
    {25, 11, 6145}, {26, 12, 8193}, {27, 12, 12289},{28, 13, 16385},{29, 13, 24577},
};

/*
 * parse_int - parses up to 4 bytes from the stream. MSB
 */
static uint32_t parse_int(const unsigned char* buffer, int n)
{
    assert(n <= 4);

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

static void decode(const unsigned char* buffer)
{
    int ll_index = -1;
    uint32_t code = 0;
    uint32_t code_len = 0;
    while (true)
    {
        code = (code << 1) + parse_bits(buffer, 1);
        code_len++;

        for (int i = 0; i < ll_alphabet.size; i++)
        {
            if (ll_alphabet.lengths[i] == code_len && ll_alphabet.codes[i] == code)
            {
                ll_index = i;
                break;
            }
        }

        if (ll_index == -1)
            continue;

        if (ll_index <= 255)
        {
            result[result_index] = ll_index;
            result_index++;
            printf("index: %d, value: %d\n", result_index, ll_index);
        }
        else if (ll_index == 256)
            break;

        else
        {
            /* parse len */
            printf("found length char %d\n", ll_index);
            uint32_t len = 0;
            for (int i = 0; i < 29; i++)
            {
                if (ll_map[i][0] != ll_index)
                    continue;

                len = ll_map[i][2] + parse_bits(buffer, ll_map[i][1]);
                printf("length is %d\n", len);
            }

            /* parse distance */
            int d_index = -1;
            uint32_t distance = 0;
            uint32_t d_code = 0;
            uint32_t d_code_len = 0;
            while(true)
            {
                d_code = (d_code << 1) + parse_bits(buffer, 1);
                d_code_len++;

                for (int i = 0; i < d_alphabet.size; i++)
                {
                    if (d_alphabet.lengths[i] == d_code_len && d_alphabet.codes[i] == d_code)
                    {
                        d_index = i;
                        break;
                    }
                }

                if (d_index == -1)
                    continue;

                for (int i = 0; i < 30; i++)
                {
                    if (d_map[i][0] != d_index)
                        continue;

                    distance = d_map[i][2] + parse_bits(buffer, d_map[i][1]);
                    break;
                }

                if (distance != 0)
                    break;
            }
            /* TODO: handle multi block distance */
            int start = result_index - distance;
            printf("%d - %d / %d\n", start, (start + len), len);
            for (int i = start; i < start + len; i++)
            {
                result[result_index] = result[i];
                printf("index: %d, value: %d\n", result_index, result[i]);
                result_index++;
            }
            printf("distance is %d\n", distance);
        }

        code = 0;
        ll_index = -1;
        code_len = 0;
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
            continue;

        alphabet->codes[i] = base_vals[code_len];
        base_vals[code_len]++;
    }
}

static void parse_alphabet(const unsigned char* buffer, alphabet_t* alphabet)
{
    int cl_index = -1;
    uint32_t index = 0;
    uint32_t code_len = 0;
    uint32_t code = 0;
    uint32_t temp_buf = 0;

    while (index < alphabet->size)
    {
        /* parse code (MSB) */
        code = (code << 1) + parse_bits(buffer, 1);
        code_len++;

        /* check if code matches any of the cl codes*/
        for (int i = 0; i < cl_alphabet.size; i++)
        {
            if (cl_alphabet.lengths[i] == code_len && cl_alphabet.codes[i] == code)
            {
                cl_index = i;
                break;
            }
        }

        /* continue parsing bits if the current code is not in the table */
        if (cl_index == -1)
            continue;

        if (cl_index <= 15)
        {
            alphabet->lengths[index] = cl_index;
            index += 1;
        }
        else if (cl_index == 16)
        {
            uint32_t count = parse_bits(buffer, 2) + 3;
            
            for (int i = index; i < (index + count); i++)
                alphabet->lengths[i] = alphabet->lengths[index - 1];

            index += count;
        }
        else if (cl_index == 17)
            index += parse_bits(buffer, 3) + 3;

        else
            index += parse_bits(buffer, 7) + 11;

        code = 0;
        cl_index = -1;
        code_len = 0;
    }

    generate_huffman_codes(alphabet);
}

static void parse_cl_alphabet(const unsigned char* buffer, int cl_size)
{
    cl_alphabet.size = 19;
    const uint8_t order[] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };

    /* parse cl lengths and count the lengths*/
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
