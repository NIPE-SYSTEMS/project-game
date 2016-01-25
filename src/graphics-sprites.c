#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "graphics-sprites.h"
#include "core.h"

static graphics_sprites_sprite_t graphics_sprites_sprites[] =
{
	{ GRAPHICS_SPRITES_TYPE_UNDESTROYABLE, "assets/undestructable.sprite", 5, 3, NULL },
	{ GRAPHICS_SPRITES_TYPE_DESTROYABLE, "assets/destructable.sprite", 5, 3, NULL },
	{ GRAPHICS_SPRITES_TYPE_DESTROYED, "assets/destroyed.sprite", 5, 3, NULL },
	{ GRAPHICS_SPRITES_TYPE_PLAYER, "assets/player.sprite", 5, 3, NULL },
	{ GRAPHICS_SPRITES_TYPE_PLAYER_STANDING, "assets/player_standing.sprite", 5, 3, NULL },
	{ GRAPHICS_SPRITES_TYPE_ENEMY, "assets/enemy.sprite", 5, 3, NULL },
	{ GRAPHICS_SPRITES_TYPE_ENEMY_STANDING, "assets/enemy_standing.sprite", 5, 3, NULL },
	{ GRAPHICS_SPRITES_TYPE_BOMB, "assets/bomb.sprite", 5, 3, NULL },
	{ GRAPHICS_SPRITES_TYPE_BOMB_UP, "assets/bomb_up.sprite", 5, 3, NULL },
	{ GRAPHICS_SPRITES_TYPE_FIRE, "assets/fire.sprite", 5, 3, NULL },
	{ GRAPHICS_SPRITES_TYPE_HEART, "assets/heart.sprite", 5, 3, NULL },
	{ GRAPHICS_SPRITES_TYPE_SNEAKER, "assets/sneaker.sprite", 5, 3, NULL },
	{ GRAPHICS_SPRITES_TYPE_UP, "assets/one_up.sprite", 5, 3, NULL },
	{ GRAPHICS_SPRITES_TYPE_SHIELD, "assets/shield.sprite", 5, 3, NULL },
	{ GRAPHICS_SPRITES_TYPE_EXPLOSION_1, "assets/explosion_1.sprite", 5, 3, NULL },
	{ GRAPHICS_SPRITES_TYPE_EXPLOSION_2, "assets/explosion_2.sprite", 5, 3, NULL },
	{ GRAPHICS_SPRITES_TYPE_SPEED, "assets/speed.sprite", 5, 3, NULL },
	{ GRAPHICS_SPRITES_TYPE_BOMBS_PLACABLE, "assets/bombs_placed.sprite", 5, 3, NULL }
};

void graphics_sprites_init_colors(void)
{
	init_pair(GRAPHICS_SPRITES_COLOR_BLACK, COLOR_BLACK, -1);
	init_pair(GRAPHICS_SPRITES_COLOR_RED, COLOR_RED, -1);
	init_pair(GRAPHICS_SPRITES_COLOR_GREEN, COLOR_GREEN, -1);
	init_pair(GRAPHICS_SPRITES_COLOR_YELLOW, COLOR_YELLOW, -1);
	init_pair(GRAPHICS_SPRITES_COLOR_BLUE, COLOR_BLUE, -1);
	init_pair(GRAPHICS_SPRITES_COLOR_MAGENTA, COLOR_MAGENTA, -1);
	init_pair(GRAPHICS_SPRITES_COLOR_CYAN, COLOR_CYAN, -1);
	init_pair(GRAPHICS_SPRITES_COLOR_WHITE, COLOR_WHITE, -1);
}

char *graphics_sprites_read(char *path, int width, int height)
{
	char *data = NULL;
	FILE *file = NULL;
	int i = 0;
	char character = 0;
	
	if(path == NULL)
	{
		return NULL;
	}
	
	core_debug("Reading sprite: %s", path);
	
	file = fopen(path, "r");
	if(file == NULL)
	{
		core_error("Failed to open sprite: %s", path);
		return NULL;
	}
	
	data = malloc(width * height * sizeof(char));
	if(data == NULL)
	{
		core_error("Failed to allocate sprite.");
		return NULL;
	}
	
	for(i = 0; i < width * height && !feof(file); i++)
	{
		character = fgetc(file);
		
		// ignore special characters
		if(character == '\n' || character == '\r')
		{
			i--;
			continue;
		}
		
		// save character
		data[i] = character;
	}
	
	fclose(file);
	
	return data;
}

void graphics_sprites_init(void)
{
	int i = 0;
	
	graphics_sprites_init_colors();
	
	for(i = 0; i < (int)(sizeof(graphics_sprites_sprites) / sizeof(graphics_sprites_sprites[0])); i++)
	{
		graphics_sprites_sprites[i].data = graphics_sprites_read(graphics_sprites_sprites[i].path, graphics_sprites_sprites[i].width, graphics_sprites_sprites[i].height);
	}
}

void graphics_sprites_cleanup(void)
{
	int i = 0;
	
	for(i = 0; i < (int)(sizeof(graphics_sprites_sprites) / sizeof(graphics_sprites_sprites[0])); i++)
	{
		free(graphics_sprites_sprites[i].data);
	}
}

graphics_sprites_sprite_t *graphics_sprites_get(graphics_sprites_type_t type)
{
	int i = 0;
	
	for(i = 0; i < (int)(sizeof(graphics_sprites_sprites) / sizeof(graphics_sprites_sprites[0])); i++)
	{
		if(graphics_sprites_sprites[i].type == type)
		{
			return &(graphics_sprites_sprites[i]);
		}
	}
	
	return NULL;
}

void graphics_sprites_render(int render_x, int render_y, graphics_sprites_type_t type, char transparency)
{
	int x = 0;
	int y = 0;
	graphics_sprites_sprite_t *sprite = NULL;
	
	sprite = graphics_sprites_get(type);
	if(sprite == NULL || sprite->data == NULL)
	{
		return;
	}
	
	for(y = 0; y < sprite->height; y++)
	{
		for(x = 0; x < sprite->width; x++)
		{
			// if transparency == 1 then render only if character of sprite != ' '
			if(transparency == 0 || (transparency == 1 && sprite->data[y * sprite->width + x] != ' '))
			{
				// TODO: Turbo-Mode colors
				mvaddch(render_y + y, render_x + x, sprite->data[y * sprite->width + x]);
			}
		}
	}
}
