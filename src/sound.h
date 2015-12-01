#ifndef __SOUND_H__
#define __SOUND_H__

#include <AL/al.h>

#define SOUND_LISTENER_DISTANCE 1.0
#define SOUND_SOURCE_MAX_X 1.0

typedef struct sound_riff_header_t
{
	char chunk_id[4];
	int chunk_size;
	char format[4];
} sound_riff_header_t;

typedef struct sound_wave_format_t
{
	char sub_chunk_id[4];
	int sub_chunk_size;
	short audio_format;
	short num_channels;
	int sample_rate;
	int byte_rate;
	short block_align;
	short bits_per_sample;
} sound_wave_format_t;

typedef struct sound_wave_data_t
{
	char sub_chunk_id[4];
	int sub_chunk_2_size;
} sound_wave_data_t;

int sound_initialize(void);
void sound_cleanup(void);
void sound_listener_initialize(void);
ALuint sound_source_initialize(char *path);
void sound_source_position_set(ALuint source, ALfloat x);
void sound_source_cleanup(ALuint source);
void sound_test(void);

#endif /* __SOUND_H__ */