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

uint32_t crc_bitwise(const unsigned char* buffer, 
                     const uint32_t size, 
                     const uint32_t poly, 
                     const uint32_t init,
                     const uint32_t config)
{
    printf("%d\n", config);
    bool xor        = false;
    uint32_t crc    = init;

    for (uint32_t i = 0; i < size; i++)
    {
        crc ^= buffer[i] << 24;

        for (uint32_t j = 0; j < 8; j++)
        {
            xor = crc & CRC_32_MSB_ON;
            crc = crc << 1;
            crc = xor ? crc ^ poly : crc;
        }
    }

    return crc;
}


/********************/
/* public functions */
/********************/

uint32_t crc(const unsigned char* buffer, 
             const uint32_t size, 
             const uint32_t poly, 
             const uint32_t init,
             const uint32_t config)
{
    if (config & CRC_BITWISE)
    {
        return crc_bitwise(buffer, size, poly, init, config);
    }
    return 0;
}