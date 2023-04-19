#include "test_time.h"

#include "test_utils.h"

#include "../time_utils.h"

#include <stdio.h>

const int64_t delta_nsec = 200;

static void test_time_now(void)
{
    timepoint_t actual = time_now();
    timepoint_t expected;
    clock_gettime(CLOCK_REALTIME, &expected);

    ASSERT_INT(expected.tv_sec, actual.tv_sec);
    ASSERT_INT_NEAR(expected.tv_nsec, actual.tv_nsec, delta_nsec);
}

static void test_time_diff_when_sec_diff_is_positive_and_nsec_diff_is_positive(void)
{
    timepoint_t a = { 100, 100 };
    timepoint_t b = { 200, 200 };
    timepoint_t actual = time_diff(a, b);
    timepoint_t expected = { 100, 100 };

    ASSERT_INT(expected.tv_sec, actual.tv_sec);
    ASSERT_INT(expected.tv_nsec, actual.tv_nsec);
}

static void test_time_diff_when_sec_diff_is_positive_and_nsec_diff_is_negative(void)
{
    timepoint_t a = { 100, 200 };
    timepoint_t b = { 200, 100 };
    timepoint_t actual = time_diff(a, b);
    timepoint_t expected = { 99, 1 * 1000 * 1000 * 1000 - 100 };

    ASSERT_INT(expected.tv_sec, actual.tv_sec);
    ASSERT_INT(expected.tv_nsec, actual.tv_nsec);
}

static void test_time_diff_when_sec_diff_is_negative_and_nsec_diff_is_negative(void)
{
    timepoint_t a = { 200, 200 };
    timepoint_t b = { 100, 100 };
    timepoint_t actual = time_diff(a, b);
    timepoint_t expected = { -101, 1 * 1000 * 1000 * 1000 - 100 };

    ASSERT_INT(expected.tv_sec, actual.tv_sec);
    ASSERT_INT(expected.tv_nsec, actual.tv_nsec);
}

static void test_time_diff_when_sec_diff_is_negative_and_nsec_diff_is_positive(void)
{
    timepoint_t a = { 200, 200 };
    timepoint_t b = { 100, 300 };
    timepoint_t actual = time_diff(a, b);
    timepoint_t expected = { -100, 100 };

    ASSERT_INT(expected.tv_sec, actual.tv_sec);
    ASSERT_INT(expected.tv_nsec, actual.tv_nsec);
}

void test_time(void)
{
    test_time_now();
    test_time_diff_when_sec_diff_is_positive_and_nsec_diff_is_positive();
    test_time_diff_when_sec_diff_is_positive_and_nsec_diff_is_negative();
    test_time_diff_when_sec_diff_is_negative_and_nsec_diff_is_negative();
    test_time_diff_when_sec_diff_is_negative_and_nsec_diff_is_positive();
}