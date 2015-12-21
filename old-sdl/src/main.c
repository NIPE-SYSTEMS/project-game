#include <stdlib.h>

#include "core.h"

int main(void)
{
	if(core_initialize() < 0)
	{
		return -1;
	}
	
	core_main();
	
	core_cleanup();
	
	return 0;
}
