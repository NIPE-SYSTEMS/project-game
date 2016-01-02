#ifndef __SPRITE_H__
#define __SPRITE_H__

typedef struct sprite_t
{
	unsigned int amount;
	unsigned int width;
	unsigned int height;
	unsigned char *data;
} sprite_t;

sprite_t *sprites_load(char *path);
void sprites_free(sprite_t *sprite);

#endif /* __SPRITE_H__ */