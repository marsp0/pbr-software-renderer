#include "time_utils.h"

#include <stdint.h>
#include <stdbool.h>

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

static timestamp_t offset 	= 0;
static bool initialized 	= false;

/********************/
/* static functions */
/********************/

/********************/
/* public functions */
/********************/

void time_init()
{
	if (initialized)
	{
		return;
	}

	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	offset = time.tv_sec;
	initialized = true;
}

timestamp_t time_now()
{
    // TODO: Add warning that time_now is being used before time_init

	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	
	return (time.tv_sec - offset) * SECOND + time.tv_nsec;
}