#pragma once

#include <stdint.h>

#define CRC_32_POLY             0x04C11DB7
#define CRC_32_INIT 			0xFFFFFFFF
#define CRC_BITWISE 			1U << 0
#define CRC_REFLECT_INPUT 		1U << 1
#define CRC_REFLECT_OUTPUT 		1U << 2

typedef struct
{
	const unsigned char* 	buffer;
	uint32_t	 			size;
	uint32_t 				poly;
	uint32_t 				init;
	uint32_t				final;
	uint32_t 				config;
} crc_input_t;

uint32_t crc(const crc_input_t input);