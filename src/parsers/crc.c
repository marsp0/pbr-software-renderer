#include "crc.h"

#include <stdio.h>
#include <stdbool.h>

/********************/
/*      defines     */
/********************/

#define CRC_32_MSB_ON       0x80000000

/********************/
/* static variables */
/********************/



/********************/
/* static functions */
/********************/

uint32_t crc_bitwise(const crc_input_t input)
{
    printf("%d\n", input.config);
    bool xor        = false;
    uint32_t crc    = input.init;

    for (uint32_t i = 0; i < input.size; i++)
    {
        crc ^= input.buffer[i] << 24;

        for (uint32_t j = 0; j < 8; j++)
        {
            xor = crc & CRC_32_MSB_ON;
            crc = crc << 1;
            crc = xor ? crc ^ input.poly : crc;
        }
    }

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
    return 0;
}