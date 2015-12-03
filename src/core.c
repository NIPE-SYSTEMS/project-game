#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>

#include "core.h"
#include "timing.h"
#include "sound.h"
#include "unused.h"
#include "rendering.h"

static SDL_Window *core_window = NULL;
static SDL_Renderer *core_renderer = NULL;
static char core_running = 0;
static double core_last_tick = 0;
static int core_refresh_rate = 0;
// static int core_fullscreen = 0;

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
	UNUSED(userdata);
	UNUSED(category);
	
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
		default: break;
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
	
	sound_initialize();
	
	core_last_tick = timing_timestamp_get();
	
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
	SDL_Event event;
	double current_tick = 0;
	int vsync_wait = 0;
	int vsync_wait_waited_frames = 0;
	int vsync_wait_dropped_frames = 0;
	int vsync_wait_total_frames = 0;
	double interval_last = 0;
	
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
		current_tick = timing_timestamp_get();
		
		while(SDL_PollEvent(&event) != 0)
		{
			switch(event.type)
			{
				case SDL_QUIT:
				{
					SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Got quit event. Initiating shutdown.");
					core_running = 0;
					
					break;
				}
				case SDL_KEYDOWN: case SDL_KEYUP:
				{
					SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "KeyEvent");
					
					if(event.key.type == SDL_KEYUP && event.key.keysym.sym == SDLK_F11)
					{
						SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Toggle fullscreen");
						SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Fullscreen not implemented.");
						
						// if(core_fullscreen == 0)
						// {
						// 	SDL_SetWindowFullscreen(core_window, SDL_WINDOW_FULLSCREEN);
						// 	core_fullscreen = 1;
						// }
						// else
						// {
						// 	SDL_SetWindowFullscreen(core_window, 0);
						// 	core_fullscreen = 0;
						// }
					}
					
					break;
				}
				default: break;
			}
		}
		
		// possible jump out of the main loop if the user requests a quit
		if(core_running == 0)
		{
			break;
		}
		
		// interval called every second
		if(timing_timestamp_get() - interval_last > 1)
		{
			// run defined interval callbacks
			timing_interval_callbacks_run();
			
			// print dropped frames error
			if(vsync_wait_dropped_frames > 0)
			{
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%i of %i frames dropped, %i frames waited", vsync_wait_dropped_frames, vsync_wait_total_frames, vsync_wait_waited_frames);
				vsync_wait_waited_frames = 0;
				vsync_wait_dropped_frames = 0;
				vsync_wait_total_frames = 0;
			}
			
			interval_last = timing_timestamp_get();
		}
		
		// clear surface with white
		SDL_SetRenderDrawColor(core_renderer, 255, 255, 255, 255);
		SDL_RenderClear(core_renderer);
		
		// render the current frame
		rendering_main(core_renderer, current_tick - core_last_tick);
		
		// wait the rest of the frame to prevent busy waiting in SDL_RenderPresent()
		vsync_wait = (((double)1 / core_refresh_rate) - timing_timestamp_get() + current_tick) * 1000 - CORE_VSYNC_BUSY_WAITING;
		vsync_wait_total_frames++;
		if(vsync_wait < 0)
		{
			vsync_wait_dropped_frames++;
		}
		else
		{
			vsync_wait_waited_frames++;
			SDL_Delay(vsync_wait);
		}
		
		SDL_RenderPresent(core_renderer);
		
		core_last_tick = current_tick;
	}
}
