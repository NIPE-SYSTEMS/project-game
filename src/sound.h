#ifndef __SOUND_H__
#define __SOUND_H__

#define AUDIO_FREQUENCY 44100
#define AUDIO_FORMAT AUDIO_S16SYS
#define AUDIO_CHANNELS 2
#define AUDIO_BUFFER 4096

int sound_initialize(void);
void sound_cleanup(void);

#endif /* __SOUND_H__ */