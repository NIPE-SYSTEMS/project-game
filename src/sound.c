#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "sound.h"

/**
 * This function initializes a new audio device of SDL_mixer.
 * 
 * @return 0 on success, -1 on error
 */
int sound_initialize(void)
{
	const SDL_version *sdl_mixer_version = Mix_Linked_Version();
	
	if(Mix_OpenAudio(AUDIO_FREQUENCY, AUDIO_FORMAT, AUDIO_CHANNELS, AUDIO_BUFFER) < 0)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_mixer: %s\n", Mix_GetError());
		sound_cleanup();
		return -1;
	}
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "SDL_mixer initialized");
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "SDL_mixer version: %i.%i.%i", sdl_mixer_version->major, sdl_mixer_version->minor, sdl_mixer_version->patch);
	
	return 0;
}

void sound_cleanup(void)
{
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Shutting down SDL_mixer...");
	
	Mix_CloseAudio();
}
