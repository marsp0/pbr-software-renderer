#include "crc.h"

#include <stdbool.h>

/********************
 *  Notes
 *
 *  1. Sunshine tutorial - sunshine2k.de/articles/coding/crc/understanding_crc.html
 *  2. Sunshine calculator - sunshine2k.de/coding/javascript/crc/crc_js.html
 *  3. CRC tutorial with info on Normal vs Reversed polynomials - github.com/michaelangel007/crc32
 *
 ********************/

/********************/
/*      defines     */
/********************/

#define CRC_32_MSB_ON   0x80000000

/********************/
/* static variables */
/********************/

static uint32_t poly_table[256] = { 0 };
static uint32_t reflect_table[256] = { 0 };

/********************/
/* static functions */
/********************/

static uint32_t reflect(uint32_t input, uint32_t size)
{
    uint32_t output = 0;
    uint32_t last = size - 1;

    for (uint32_t i = 0; i < size; i++)
    {
        output |= (input & (1U << i)) ? 1U << (last - i) : 0;
    }

    return output;
}

static uint32_t crc_bitwise(const crc_input_t input)
{
    bool xor        = false;
    uint32_t crc    = input.init;

    for (uint32_t i = 0; i < input.size; i++)
    {
        uint32_t b = input.buffer[i];
        if (input.config & CRC_REFLECT_INPUT)
        {
            b = reflect(input.buffer[i], 8);
        }
        
        crc ^= b << 24;

        for (uint32_t j = 0; j < 8; j++)
        {
            xor = crc & CRC_32_MSB_ON;
            crc = crc << 1;
            crc = xor ? crc ^ input.poly : crc;
        }
    }

    crc = input.config & CRC_REFLECT_OUTPUT ? reflect(crc, 32) : crc;

    return crc ^ input.final;
}

static void init_tables(crc_input_t input)
{
    // populate poly table
    bool xor = false;

    for (uint32_t i = 0; i < 256; i++)
    {
        uint32_t crc = i << 24;

        for (uint32_t j = 0; j < 8; j++)
        {
            xor = crc & CRC_32_MSB_ON;
            crc = crc << 1;
            crc = xor ? crc ^ input.poly : crc;
        }

        poly_table[i] = crc;
    }

    // populate byte reflection table
    if (input.config & CRC_REFLECT_INPUT)
    {
        for (uint32_t i = 0; i < 256; i++)
        {
            reflect_table[i] = reflect(i, 8);
        }
    }
}

static uint32_t crc_table(const crc_input_t input)
{
    init_tables(input);

    uint32_t crc = input.init;

    for (uint32_t i = 0; i < input.size; i++)
    {
        uint32_t b = input.buffer[i];
        if (input.config & CRC_REFLECT_INPUT)
        {
            b = reflect_table[input.buffer[i]];
        }

        uint32_t index = (crc ^ (b << 24)) >> 24;

        crc = (crc << 8) ^ poly_table[index];
    }

    crc = input.config & CRC_REFLECT_OUTPUT ? reflect(crc, 32) : crc;

    return crc ^ input.final;
}


/********************/
/* public functions */
/********************/

uint32_t crc(const crc_input_t input)
{
    if (input.config & CRC_BITWISE)
    {
        return crc_bitwise(input);
    }
    return crc_table(input);
}