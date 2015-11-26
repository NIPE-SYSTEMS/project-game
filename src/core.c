#include <stdio.h>
#include <SDL2/SDL.h>

#include "core.h"
#include "sound.h"

static SDL_Window *core_window = NULL;

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
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Creating window...");
	
	core_window = SDL_CreateWindow("project-game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0); // no window flags
	if(core_window == NULL)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s\n", SDL_GetError());
		core_cleanup();
		return -2;
	}
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Window created");
	
	core_surface_clear();
	
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
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Destroying window...");
	
	if(core_window != NULL)
	{
		SDL_DestroyWindow(core_window);
	}
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Window destroyed");
	
	sound_cleanup();
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Shutting down SDL...");
	
	SDL_Quit();
}

/**
 * This function clears the window with the default color (white).
 */
void core_surface_clear(void)
{
	SDL_Surface *surface = NULL;
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Clearing surface...");
	
	surface = SDL_GetWindowSurface(core_window);
	if(surface == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to retrieve window surface: %s", SDL_GetError());
		return;
	}
	
	// fill entire surface with white color
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));
	
	// update window surface
	SDL_UpdateWindowSurface(core_window);
}
