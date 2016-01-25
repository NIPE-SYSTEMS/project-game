#include <stdlib.h>

#include "core.h"

int main(void)
{
	core_init();
	core_main();
	core_cleanup();
	
	return 0;
}
