#include <stdlib.h>
#include <stdio.h>

#include "sprites.h"

sprites_t *sprites_load(char *path)
{
	FILE *sprite_file = NULL;
	sprites_t *sprite = NULL;
	
	sprite_file = fopen(path, "r");
	if(sprite_file == NULL)
	{
		return NULL;
	}
	
	sprite = calloc(1, sizeof(sprites_t));
	if(sprite == NULL)
	{
		fclose(sprite_file);
		return NULL;
	}
	
	// read metadata
	fread(&(sprite->amount), 2, 1, sprite_file);
	fread(&(sprite->width), 2, 1, sprite_file);
	fread(&(sprite->height), 2, 1, sprite_file);
	
	sprite->data = calloc(sprite->amount * sprite->width * sprite->height, sizeof(char));
	if(sprite->data == NULL)
	{
		fclose(sprite_file);
		free(sprite);
		return NULL;
	}
	
	// read sprite data
	fread(sprite->data, sprite->width * sprite->height, sprite->amount, sprite_file);
	
	fclose(sprite_file);
	
	return sprite;
}

void sprites_free(sprites_t *sprite)
{
	free(sprite->data);
	free(sprite);
}
