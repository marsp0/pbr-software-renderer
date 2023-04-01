#include "test_crc.h"

#include "test_utils.h"

#include "../parsers/crc.h"

static void test_crc_bitwise_no_init(void)
{
	const char* buffer = "123456789";
	const uint32_t size = 9;
	const uint32_t poly = CRC_32_POLY;
	const uint32_t init = 0;
	const uint32_t config = CRC_BITWISE;
	const uint32_t actual = crc(buffer, size, poly, init, config);
	const uint32_t expected = 0x89A1897F;

	ASSERT_UINT(expected, actual);
}

void test_crc_bitwise_w_init(void)
{
	const char* buffer = "123456789";
	const uint32_t size = 9;
	const uint32_t poly = CRC_32_POLY;
	const uint32_t config = CRC_BITWISE;

	// first calc
	uint32_t init = 0xF0000000;
	uint32_t actual = crc(buffer, size, poly, init, config);
	uint32_t expected = 0x02521484;

	ASSERT_UINT(expected, actual);

	// second calc
	init = 0xF000000F;
	actual = crc(buffer, size, poly, init, config);
	expected = 0x9ACA5D2C;

	ASSERT_UINT(expected, actual);

	// third calc
	init = 0xFFFFFFFF;
	actual = crc(buffer, size, poly, init, config);
	expected = 0x0376E6E7;

	ASSERT_UINT(expected, actual);
}



void test_crc(void)
{
	test_crc_bitwise_no_init();
	test_crc_bitwise_w_init();
}