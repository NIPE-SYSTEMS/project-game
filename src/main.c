#include <stdio.h>
#include <stdlib.h>

#include "core.h"

int main(void)
{
	if(core_initialize() < 0)
	{
		return -1;
	}
	
	sleep(10);
	
	core_cleanup();
	
	return 0;
}
