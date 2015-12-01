#include <AL/al.h>

#include "sound.h"

int main(void)
{
	sound_initialize();
	
	sound_test();
	
	sound_cleanup();
	
	// if(core_initialize() < 0)
	// {
	// 	return -1;
	// }
	
	// core_main();
	
	// core_cleanup();
	
	return 0;
}
