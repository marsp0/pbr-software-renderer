#include "time_utils.h"

#include <stdint.h>

/********************
 *  Notes
 *
 ********************/

/********************/
/*      defines     */
/********************/

/********************/
/* static variables */
/********************/

const int64_t one_second_ns = 1 * 1000 * 1000 * 1000;

/********************/
/* static functions */
/********************/

/********************/
/* public functions */
/********************/

timepoint_t time_now()
{
	timepoint_t time;
	clock_gettime(CLOCK_REALTIME, &time);
	return time;
}

timepoint_t time_diff(const timepoint_t before, const timepoint_t after)
{
	timepoint_t diff;

	if (after.tv_nsec < before.tv_nsec)
	{
		diff.tv_sec = after.tv_sec - before.tv_sec - 1;
		diff.tv_nsec = after.tv_nsec + one_second_ns - before.tv_nsec;
	}
	else
	{
		diff.tv_sec = after.tv_sec - before.tv_sec;
		diff.tv_nsec = after.tv_nsec - before.tv_nsec;
	}

	return diff;
}