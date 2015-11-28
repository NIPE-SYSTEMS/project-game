#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>

#include "core.h"
#include "sound.h"

static SDL_Window *core_window = NULL;
static SDL_Renderer *core_renderer = NULL;
static char core_running = 0;
static clock_t core_last_tick = 0;
static int core_refresh_rate = 0;

/**
 * This function is used to output fancy log messages. The function acts as a
 * callback function for SDL_Log[...]().
 * 
 * @param userdata what was passed as userdata to SDL_LogSetOutputFunction()
 * @param category the category of the message
 * @param priority the priority of the message
 * @param message the message being output
 */
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
 * This function displays available renderers.
 */
static void core_renderers_list(void)
{
	int i = 0;
	SDL_RendererInfo renderer_info;
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Available Renderers (amount: %i):", SDL_GetNumRenderDrivers());
	
	for(i = 0; i < SDL_GetNumRenderDrivers(); i++)
	{
		SDL_GetRenderDriverInfo(i, &renderer_info);
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "\tname: \"%s\"", renderer_info.name);
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "\tflags:");
		if((renderer_info.flags) & (1 << SDL_RENDERER_SOFTWARE))
		{
			SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "\t\tSDL_RENDERER_SOFTWARE");
		}
		if((renderer_info.flags) & (1 << SDL_RENDERER_ACCELERATED))
		{
			SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "\t\tSDL_RENDERER_ACCELERATED");
		}
		if((renderer_info.flags) & (1 << SDL_RENDERER_PRESENTVSYNC))
		{
			SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "\t\tSDL_RENDERER_PRESENTVSYNC");
		}
		if((renderer_info.flags) & (1 << SDL_RENDERER_TARGETTEXTURE))
		{
			SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "\t\tSDL_RENDERER_TARGETTEXTURE");
		}
		if(!((renderer_info.flags) & (1 << SDL_RENDERER_SOFTWARE)) && !((renderer_info.flags) & (1 << SDL_RENDERER_ACCELERATED)) && !((renderer_info.flags) & (1 << SDL_RENDERER_PRESENTVSYNC)) && !((renderer_info.flags) & (1 << SDL_RENDERER_TARGETTEXTURE)))
		{
			SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "\t\t(none)");
		}
	}
}

/**
 * This function initializes everything of SDL_mixer.
 * 
 * @return 0 on success, -1 on error
 */
int core_initialize(void)
{
	SDL_version sdl_version;
	SDL_RendererInfo renderer_info;
	SDL_DisplayMode display_mode;
	
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
	
	core_renderers_list();
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Creating renderer...");
	
	core_renderer  = SDL_CreateRenderer(core_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(core_renderer == NULL)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create renderer: %s\n", SDL_GetError());
		core_cleanup();
		return -3;
	}
	
	SDL_GetRendererInfo(core_renderer, &renderer_info);
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Choosed \"%s\" renderer", renderer_info.name);
	
	// clear surface with white and render it
	SDL_SetRenderDrawColor(core_renderer, 255, 255, 255, 255);
	SDL_RenderClear(core_renderer);
	SDL_RenderPresent(core_renderer);
	
	SDL_GetWindowDisplayMode(core_window, &display_mode);
	core_refresh_rate = display_mode.refresh_rate;
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Refresh rate: %i Hz", core_refresh_rate);
	
	// SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
	// SDL_RenderSetLogicalSize(core_renderer, 1280 * 2, 720 * 2);
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Renderer created");
	
	if(sound_initialize() < 0)
	{
		core_cleanup();
	}
	
	core_last_tick = clock();
	
	return 0;
}

/**
 * This function cleans everything up.
 */
void core_cleanup(void)
{
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Destroying renderer...");
	
	if(core_renderer != NULL)
	{
		SDL_DestroyRenderer(core_renderer);
	}
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Renderer destroyed");
	
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
 * This function acts as a main loop in the game. This main loop processes input
 * events and draws something on the screen.
 */
void core_main(void)
{
	SDL_Surface *surface = NULL;
	SDL_Event event;
	clock_t current_tick = 0;
	int vsync_wait = 0;
	
	SDL_Rect rect0 = { 100, 100, 100, 100 };
	SDL_Rect rect1 = { 100, 100, 100, 100 };
	SDL_Rect rect2 = { 100, 100, 100, 100 };
	SDL_Rect rect3 = { 100, 100, 100, 100 };
	SDL_Rect rect4 = { 100, 100, 100, 100 };
	
	if(core_renderer == NULL)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Invalid renderer in main loop");
		return;
	}
	
	core_running = 1;
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Starting main loop...");
	
	srand(time(NULL));
	
	while(core_running == 1)
	{
		current_tick = clock();
		
		while(SDL_PollEvent(&event) != 0)
		{
			if(event.type == SDL_QUIT)
			{
				SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Got quit event. Initiating shutdown.");
				core_running = 0;
			}
		}
		
		// possible jump out of the main loop if the user requests a quit
		if(core_running == 0)
		{
			break;
		}
		
		// clear surface with white
		SDL_SetRenderDrawColor(core_renderer, 255, 255, 255, 255);
		SDL_RenderClear(core_renderer);
		
		// update rectangle positions to random coordinates
		rect0.x = rand() % 1180;
		rect0.y = rand() % 620;
		rect1.x = rand() % 1180;
		rect1.y = rand() % 620;
		rect2.x = rand() % 1180;
		rect2.y = rand() % 620;
		rect3.x = rand() % 1180;
		rect3.y = rand() % 620;
		rect4.x = rand() % 1180;
		rect4.y = rand() % 620;
		
		// actually render rectangles
		SDL_SetRenderDrawColor(core_renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(core_renderer, &rect0);
		SDL_RenderFillRect(core_renderer, &rect1);
		SDL_RenderFillRect(core_renderer, &rect2);
		SDL_RenderFillRect(core_renderer, &rect3);
		SDL_RenderFillRect(core_renderer, &rect4);
		
		// wait the rest of the frame to prevent busy waiting in SDL_RenderPresent()
		vsync_wait = (((float)1 / core_refresh_rate) - (float)(clock() - current_tick) / CLOCKS_PER_SEC - CORE_VSYNC_BUSY_WAITING) * 1000;
		SDL_Delay((vsync_wait < 0)?(0):(vsync_wait));
		
		SDL_RenderPresent(core_renderer);
		
		core_last_tick = current_tick;
	}
}
