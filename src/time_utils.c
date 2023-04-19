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

static timestamp_t offset = 0;

/********************/
/* static functions */
/********************/

/********************/
/* public functions */
/********************/

void time_init()
{
	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);

	offset = time.tv_sec;
}

timestamp_t time_now()
{
	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	
	return (time.tv_sec - offset) * SECOND + time.tv_nsec;
}