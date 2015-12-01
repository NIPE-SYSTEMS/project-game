#include <stdio.h>
#include <SDL2/SDL.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <unistd.h>

#include "sound.h"

static ALCdevice *device = NULL;
static ALCcontext *context = NULL;

static ALuint sound_source_load(char *path)
{
	FILE *sound_file = NULL;
	sound_riff_header_t sound_riff_header;
	sound_wave_format_t sound_wave_format;
	sound_wave_data_t sound_wave_data;
	unsigned char *data = NULL;
	ALsizei size = 0;
	ALsizei frequency = 0;
	ALenum format = AL_FORMAT_MONO8;
	ALuint buffer = 0;
	size_t fread_return = 0;
	
	if(context == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid context");
	}
	
	if(path == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid path of sound file.");
	}
	
	sound_file = fopen(path, "rb");
	if(sound_file == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open sound file: %s", path);
		return AL_NONE;
	}
	
	if(!(fread_return = fread(&sound_riff_header, sizeof(sound_riff_header_t), 1, sound_file)))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to read RIFF header from sound file: %s (fread-return: %li)", path, fread_return);
		fclose(sound_file);
		return AL_NONE;
	}
	
	if((sound_riff_header.chunk_id[0] != 'R' || sound_riff_header.chunk_id[1] != 'I' || sound_riff_header.chunk_id[2] != 'F' || sound_riff_header.chunk_id[3] != 'F') || (sound_riff_header.format[0] != 'W' || sound_riff_header.format[1] != 'A' || sound_riff_header.format[2] != 'V' || sound_riff_header.format[3] != 'E'))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid RIFF or WAVE header in sound file: %s", path);
		fclose(sound_file);
		return AL_NONE;
	}
	
	if(!(fread_return = fread(&sound_wave_format, sizeof(sound_wave_format_t), 1, sound_file)))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to read WAVE format from sound file: %s (fread-return: %li)", path, fread_return);
		fclose(sound_file);
		return AL_NONE;
	}
	
	if(sound_wave_format.sub_chunk_id[0] != 'f' || sound_wave_format.sub_chunk_id[1] != 'm' || sound_wave_format.sub_chunk_id[2] != 't' || sound_wave_format.sub_chunk_id[3] != ' ')
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid WAVE format in sound file: %s", path);
		fclose(sound_file);
		return AL_NONE;
	}
	
	if(sound_wave_format.sub_chunk_size > 16)
	{
		fseek(sound_file, sizeof(short), SEEK_CUR);
	}
	
	if(!(fread_return = fread(&sound_wave_data, sizeof(sound_wave_data_t), 1, sound_file)))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to read WAVE data from sound file: %s (fread-return: %li)", path, fread_return);
		fclose(sound_file);
		return AL_NONE;
	}
	
	data = malloc(sound_wave_data.sub_chunk_2_size * sizeof(char));
	if(data == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to allocate memory for sound file: %s", path);
		fclose(sound_file);
		return AL_NONE;
	}
	
	if(!(fread_return = fread(data, sound_wave_data.sub_chunk_2_size, 1, sound_file)))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to read data from sound file: %s (fread-return: %li)", path, fread_return);
		fclose(sound_file);
		free(data);
		return AL_NONE;
	}
	
	size = sound_wave_data.sub_chunk_2_size;
	frequency = sound_wave_format.sample_rate;
	
	if(sound_wave_format.num_channels == 1)
	{
		if(sound_wave_format.bits_per_sample == 8 )
		{
			format = AL_FORMAT_MONO8;
		}
		else if(sound_wave_format.bits_per_sample == 16)
		{
			format = AL_FORMAT_MONO16;
		}
	}
	else if(sound_wave_format.num_channels == 2)
	{
		if(sound_wave_format.bits_per_sample == 8 )
		{
			format = AL_FORMAT_STEREO8;
		}
		else if(sound_wave_format.bits_per_sample == 16)
		{
			format = AL_FORMAT_STEREO16;
		}
	}
	
	alGenBuffers(1, &buffer);
	if(alGetError() != AL_NO_ERROR)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to generate buffer for sound file: %s", path);
		fclose(sound_file);
		free(data);
		return AL_NONE;
	}
	
	alBufferData(buffer, format, (void *)data, size, frequency);
	
	free(data);
	fclose(sound_file);
	
	return buffer;
}

int sound_initialize(void)
{
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Initializing sound...");
	
	device = alcOpenDevice(NULL);
	if(device == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open audio device.");
		return -1;
	}
	
	context = alcCreateContext(device, NULL);
	if(context == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create audio context.");
		alcCloseDevice(device);
		device = NULL;
		return -2;
	}
	
	if(alcMakeContextCurrent(context) == AL_FALSE)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to make context current.");
		alcDestroyContext(context);
		alcCloseDevice(device);
		device = NULL;
		context = NULL;
		return -3;
	}
	
	sound_listener_initialize();
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Sound initialized");
	
	return 0;
}

void sound_cleanup(void)
{
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Shutting down sound...");
	
	if(context != NULL)
	{
		alcDestroyContext(context);
	}
	
	if(device != NULL)
	{
		alcCloseDevice(device);
	}
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Sound is shut down");
}

void sound_listener_initialize(void)
{
	ALfloat listener_position[] = { 0.0, 0.0, SOUND_LISTENER_DISTANCE };
	ALfloat listener_velocity[] = { 0.0, 0.0, 0.0 };
	ALfloat listener_orientation[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 }; // at-vector, up-vector
	
	alListenerfv(AL_POSITION, listener_position);
	alListenerfv(AL_VELOCITY, listener_velocity);
	alListenerfv(AL_ORIENTATION, listener_orientation);
	
	alDistanceModel(AL_EXPONENT_DISTANCE);
}

ALuint sound_source_initialize(char *path)
{
	ALuint source = 0;
	ALuint source_buffer = 0;
	ALfloat source_position[] = { 0.0, 0.0, 0.0 };
	ALfloat source_velocity[] = { 0.0, 0.0, 0.0 };
	
	if(context == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid context");
	}
	
	source_buffer = sound_source_load(path);
	if(source_buffer == AL_NONE)
	{
		return AL_NONE;
	}
	
	alGenSources(1, &source);
	if(alGetError() != AL_NO_ERROR)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to generate source.");
		alDeleteBuffers(1, &source_buffer);
		return AL_NONE;
	}
	
	alSourcei(source, AL_BUFFER, source_buffer);
	alSourcef(source, AL_PITCH, 1.0);
	alSourcef(source, AL_GAIN, 1.0);
	alSourcefv(source, AL_POSITION, source_position);
	alSourcefv(source, AL_VELOCITY, source_velocity);
	alSourcei(source, AL_LOOPING, AL_FALSE);
	alSourcef(source, AL_REFERENCE_DISTANCE, 1.0);
	alSourcef(source, AL_ROLLOFF_FACTOR, 2.0);
	
	if(alGetError() != AL_NO_ERROR)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to set source parameters.");
		alDeleteSources(1, &source);
		alDeleteBuffers(1, &source_buffer);
		return AL_NONE;
	}
	
	return source;
}

void sound_source_position_set(ALuint source, ALfloat x) // [-1, 1]
{
	ALfloat source_position[] = { x * SOUND_SOURCE_MAX_X, 0.0, 0.0 };
	
	if(context == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid context");
	}
	
	alSourcefv(source, AL_POSITION, source_position);
}

void sound_source_cleanup(ALuint source)
{
	ALuint source_buffer = 0;
	ALint source_buffer_temp = 0;
	
	if(context == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid context");
	}
	
	alGetSourcei(source, AL_BUFFER, &source_buffer_temp);
	source_buffer = source_buffer_temp;
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &source_buffer);
}

void sound_test(void)
{
	ALuint source;
	ALint state;
	ALfloat position = 0;
	ALfloat step = 0.01;
	
	source = sound_source_initialize("res/sound/test.wav");
	if(source == AL_NONE)
	{
		return;
	}
	
	alSourcePlay(source);
	// sound_source_position_set(source, 0.0);
	
	do
	{
		alGetSourcei(source, AL_SOURCE_STATE, &state);
		if(position > 1 || position < 0)
		{
			step = -step;
		}
		position += step;
		sound_source_position_set(source, position);
		// printf("%f\n", source0_position[0]);
		usleep(10000);
	}
	while(state == AL_PLAYING);
	
	sound_source_cleanup(source);
}
