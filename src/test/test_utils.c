#include "test_utils.h"

#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <string.h>

static uint32_t assert_count = 0;
static uint32_t passed = 0;
static uint32_t failed = 0;

void reset_test_assert_counter()
{
    if (assert_count == 0)
    {
        passed += 1;
    }
    assert_count = 0;
}

void increment_test_assert_counter()
{
    if (assert_count == 0)
    {
        failed += 1;
    }
    assert_count++;
}

uint32_t get_test_assert_counter()
{
    return assert_count;
}

void TESTS_INIT()
{
    passed = 0;
    failed = 0;
}

uint32_t TESTS_FAIL_COUNT()
{
    return failed;
}

void TESTS_SUMMARY()
{
    printf("\n");
    printf("+----------------------------------------+\n");
    printf("|              Passed: %3d               |\n", passed);
    printf("|              Failed: %3d               |\n", failed);
    printf("+----------------------------------------+\n");
}