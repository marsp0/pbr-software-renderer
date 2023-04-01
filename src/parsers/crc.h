#pragma once

#include <stdint.h>

#define CRC_32_POLY             0x04C11DB7
#define CRC_32_INIT 			0xFFFFFFFF
#define CRC_BITWISE 			1U << 0
#define CRC_REFLECT_INPUT 		1U << 1
#define CRC_REFLECT_OUTPUT 		1U << 2

uint32_t crc(const unsigned char* buffer, 
			 const uint32_t size, 
			 const uint32_t poly, 
			 const uint32_t init,
			 const uint32_t config);