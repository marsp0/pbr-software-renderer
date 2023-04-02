#include "test_crc.h"

#include "test_utils.h"

#include "../parsers/crc.h"

static void test_crc_bitwise_no_init(void)
{
    crc_input_t input;
    input.buffer    = "123456789";
    input.size      = 9;
    input.poly      = CRC_32_POLY;
    input.init      = 0;
    input.final     = 0;
    input.config    = CRC_BITWISE;

    const uint32_t actual = crc(input);
    const uint32_t expected = 0x89A1897F;

    ASSERT_HEX(expected, actual);
}

void test_crc_bitwise_w_init(void)
{
    crc_input_t input;
    input.buffer    = "123456789";
    input.size      = 9;
    input.poly      = CRC_32_POLY;
    input.init      = 0xF0000000;
    input.final     = 0;
    input.config    = CRC_BITWISE;

    // first calc
    uint32_t actual = crc(input);
    uint32_t expected = 0x02521484;

    ASSERT_HEX(expected, actual);

    // second calc
    input.init = 0xF000000F;
    actual = crc(input);
    expected = 0x9ACA5D2C;

    ASSERT_HEX(expected, actual);

    // third calc
    input.init = 0xFFFFFFFF;
    actual = crc(input);
    expected = 0x0376E6E7;

    ASSERT_HEX(expected, actual);
}

void test_crc_bitwise_no_init_w_final(void)
{
    crc_input_t input;
    input.buffer    = "123456789";
    input.size      = 9;
    input.poly      = CRC_32_POLY;
    input.init      = 0;
    input.final     = 0xFFFFFFFF;
    input.config    = CRC_BITWISE;

    const uint32_t actual = crc(input);
    const uint32_t expected = 0x765E7680;

    ASSERT_HEX(expected, actual);
}

void test_crc_bitwise_w_init_w_final(void)
{
    crc_input_t input;
    input.buffer    = "123456789";
    input.size      = 9;
    input.poly      = CRC_32_POLY;
    input.init      = 0xFFFFFFFF;
    input.final     = 0xEDEDEDED;
    input.config    = CRC_BITWISE;

    const uint32_t actual = crc(input);
    const uint32_t expected = 0xEE9B0B0A;

    ASSERT_HEX(expected, actual);
}

void test_crc_bitwise_no_init_no_final_reflected(void)
{
    crc_input_t input;
    input.buffer    = "123456789";
    input.size      = 9;
    input.poly      = CRC_32_POLY;
    input.init      = 0;
    input.final     = 0;
    input.config    = CRC_BITWISE | CRC_REFLECT_INPUT | CRC_REFLECT_OUTPUT;

    const uint32_t actual = crc(input);
    const uint32_t expected = 0x2DFD2D88;

    ASSERT_HEX(expected, actual);
}

void test_crc_bitwise_w_init_no_final_reflected(void)
{
    crc_input_t input;
    input.buffer    = "123456789";
    input.size      = 9;
    input.poly      = CRC_32_POLY;
    input.init      = 0xF0000000;
    input.final     = 0;
    input.config    = CRC_BITWISE | CRC_REFLECT_INPUT | CRC_REFLECT_OUTPUT;

    // first calc
    uint32_t actual = crc(input);
    uint32_t expected = 0xF244E259;

    ASSERT_HEX(expected, actual);
}

void test_crc_bitwise_w_init_w_final_reflected(void)
{
    crc_input_t input;
    input.buffer    = "123456789";
    input.size      = 9;
    input.poly      = CRC_32_POLY;
    input.init      = 0xFFFFFFFF;
    input.final     = 0xEDEDEDED;
    input.config    = CRC_BITWISE | CRC_REFLECT_INPUT | CRC_REFLECT_OUTPUT;

    const uint32_t actual = crc(input);
    const uint32_t expected = 0xD9E62B34;

    ASSERT_HEX(expected, actual);
}

void test_crc(void)
{
    test_crc_bitwise_no_init();
    test_crc_bitwise_w_init();
    test_crc_bitwise_no_init_w_final();
    test_crc_bitwise_w_init_w_final();
    test_crc_bitwise_no_init_no_final_reflected();
    test_crc_bitwise_w_init_no_final_reflected();
    test_crc_bitwise_w_init_w_final_reflected();
}