#include <stdio.h>
#include <SDL2/SDL.h>

#include "core.h"
#include "sound.h"

static void core_log_output(void *userdata, int category, SDL_LogPriority priority, const char *message)
{
	switch(priority)
	{
		case SDL_LOG_PRIORITY_VERBOSE:
		{
			fprintf(stderr, "VERBOSE: ");
			break;
		}
		case SDL_LOG_PRIORITY_DEBUG:
		{
			fprintf(stderr, "DEBUG: ");
			break;
		}
		case SDL_LOG_PRIORITY_INFO:
		{
			fprintf(stderr, "INFO: ");
			break;
		}
		case SDL_LOG_PRIORITY_WARN:
		{
			fprintf(stderr, "\e[0;33mWARN: ");
			break;
		}
		case SDL_LOG_PRIORITY_ERROR:
		{
			fprintf(stderr, "\e[0;31mERROR: ");
			break;
		}
		case SDL_LOG_PRIORITY_CRITICAL:
		{
			fprintf(stderr, "\e[0;31mCRITICAL: ");
			break;
		}
	}
	
	fprintf(stderr, message);
	fprintf(stderr, "\e[0m\n");
}

/**
 * This function initializes everything of SDL_mixer.
 * 
 * @return 0 on success, -1 on error
 */
int core_initialize(void)
{
	SDL_version sdl_version;
	
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		fprintf(stderr, "Failed to intialize SDL: %s\n", SDL_GetError());
		return -1;
	}
	
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
	
	SDL_LogSetOutputFunction(core_log_output, NULL);
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "SDL initialized");
	SDL_GetVersion(&sdl_version);
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "SDL version: %i.%i.%i", sdl_version.major, sdl_version.minor, sdl_version.patch);
	
	if(sound_initialize() < 0)
	{
		core_cleanup();
	}
	
	return 0;
}

/**
 * This function cleans everything up.
 */
void core_cleanup(void)
{
	sound_cleanup();
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Shutting down SDL...");
	SDL_Quit();
}
