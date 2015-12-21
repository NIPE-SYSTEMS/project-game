#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#include "timing.h"

static timing_interval_callback_t timing_interval_callbacks[] = {  };

/**
 * This function returns a UNIX epoch timestamp as double. The timestamp has
 * microseconds precision.
 * 
 * @return the requested timestamp.
 */
double timing_timestamp_get(void)
{
	struct timeval tm = { 0, 0 };
	
	gettimeofday(&tm, NULL);
	
	return tm.tv_sec + ((double)tm.tv_usec / 1000000);
}

/**
 * This function is a helper function which executes all defined callback
 * functions every one second. It is called by the core main loop.
 */
void timing_interval_callbacks_run(void)
{
	int i = 0;
	
	// loop over all callback functions
	for(i = 0; i < (int)(sizeof(timing_interval_callbacks) / sizeof(timing_interval_callbacks[0])); i++)
	{
		// execute non-NULL functions
		if(timing_interval_callbacks[i] != NULL)
		{
			timing_interval_callbacks[i]();
		}
	}
}
