#ifndef __SPRITES_H__
#define __SPRITES_H__

typedef struct sprites_t
{
	unsigned int amount;
	unsigned int width;
	unsigned int height;
	unsigned char *data;
} sprites_t;

sprites_t *sprites_load(char *path);
void sprites_free(sprites_t *sprite);

#endif /* __SPRITES_H__ */