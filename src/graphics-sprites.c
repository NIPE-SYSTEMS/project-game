#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>

#include "graphics-sprites.h"
#include "core.h"

static graphics_sprites_sprite_t graphics_sprites_sprites[] =
{
	{ GRAPHICS_SPRITES_TYPE_UNDESTROYABLE, "assets/undestructable.sprite", 5, 3, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_DESTROYABLE, "assets/destructable.sprite", 5, 3, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_DESTROYED, "assets/destroyed.sprite", 5, 3, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_PLAYER, "assets/player.sprite", 5, 3, NULL, GRAPHICS_SPRITES_COLOR_GREEN },
	{ GRAPHICS_SPRITES_TYPE_PLAYER_STANDING, "assets/player_standing.sprite", 5, 3, NULL, GRAPHICS_SPRITES_COLOR_GREEN },
	{ GRAPHICS_SPRITES_TYPE_PLAYER_TURBO_MODE, "assets/player.sprite", 5, 3, NULL, GRAPHICS_SPRITES_COLOR_RANDOM },
	{ GRAPHICS_SPRITES_TYPE_PLAYER_STANDING_TURBO_MODE, "assets/player_standing.sprite", 5, 3, NULL, GRAPHICS_SPRITES_COLOR_RANDOM },
	{ GRAPHICS_SPRITES_TYPE_ENEMY, "assets/enemy.sprite", 5, 3, NULL, GRAPHICS_SPRITES_COLOR_RED },
	{ GRAPHICS_SPRITES_TYPE_ENEMY_STANDING, "assets/enemy_standing.sprite", 5, 3, NULL, GRAPHICS_SPRITES_COLOR_RED },
	{ GRAPHICS_SPRITES_TYPE_BOMB, "assets/bomb.sprite", 5, 3, NULL, GRAPHICS_SPRITES_COLOR_RED },
	{ GRAPHICS_SPRITES_TYPE_BOMB_UP, "assets/bomb_up.sprite", 5, 3, NULL, GRAPHICS_SPRITES_COLOR_YELLOW },
	{ GRAPHICS_SPRITES_TYPE_FIRE, "assets/fire.sprite", 5, 3, NULL, GRAPHICS_SPRITES_COLOR_YELLOW },
	{ GRAPHICS_SPRITES_TYPE_HEART, "assets/heart.sprite", 5, 3, NULL, GRAPHICS_SPRITES_COLOR_YELLOW },
	{ GRAPHICS_SPRITES_TYPE_SNEAKER, "assets/sneaker.sprite", 5, 3, NULL, GRAPHICS_SPRITES_COLOR_YELLOW },
	{ GRAPHICS_SPRITES_TYPE_UP, "assets/one_up.sprite", 5, 3, NULL, GRAPHICS_SPRITES_COLOR_YELLOW },
	{ GRAPHICS_SPRITES_TYPE_SHIELD, "assets/shield.sprite", 5, 3, NULL, GRAPHICS_SPRITES_COLOR_YELLOW },
	{ GRAPHICS_SPRITES_TYPE_EXPLOSION_1, "assets/explosion_1.sprite", 5, 3, NULL, GRAPHICS_SPRITES_COLOR_RED },
	{ GRAPHICS_SPRITES_TYPE_EXPLOSION_2, "assets/explosion_2.sprite", 5, 3, NULL, GRAPHICS_SPRITES_COLOR_YELLOW },
	{ GRAPHICS_SPRITES_TYPE_SPEED, "assets/speed.sprite", 5, 3, NULL, GRAPHICS_SPRITES_COLOR_BLUE },
	{ GRAPHICS_SPRITES_TYPE_BOMBS_PLACABLE, "assets/bombs_placed.sprite", 5, 3, NULL, GRAPHICS_SPRITES_COLOR_GREEN },
	{ GRAPHICS_SPRITES_TYPE_MENU_KEY_1, "assets/menu_key_1.sprite", 39, 5, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_MENU_KEY_2, "assets/menu_key_2.sprite", 39, 5, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_MENU_KEY_3, "assets/menu_key_3.sprite", 39, 5, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_MENU_KEY_4, "assets/menu_key_4.sprite", 39, 5, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_MENU_LOGO_1, "assets/menu_logo_1.sprite", 60, 12, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_MENU_LOGO_2, "assets/menu_logo_2.sprite", 60, 12, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_MENU_START, "assets/menu_start.sprite", 25, 1, NULL, GRAPHICS_SPRITES_COLOR_RANDOM_FILL },
	
	{ GRAPHICS_STARTSCREEN_TEXT, "assets/Field_START_Text.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_1, "assets/Field_START_1.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_2, "assets/Field_START_2.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_3, "assets/Field_START_3.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_4, "assets/Field_START_4.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_5, "assets/Field_START_5.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_6, "assets/Field_START_6.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_7, "assets/Field_START_7.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_8, "assets/Field_START_8.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_9, "assets/Field_START_9.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_10, "assets/Field_START_10.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_11, "assets/Field_START_11.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_12, "assets/Field_START_12.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_13, "assets/Field_START_13.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_14, "assets/Field_START_14.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_15, "assets/Field_START_15.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_16, "assets/Field_START_16.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_17, "assets/Field_START_17.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_18, "assets/Field_START_18.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_19, "assets/Field_START_19.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_20, "assets/Field_START_20.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_21, "assets/Field_START_21.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_22, "assets/Field_START_22.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_23, "assets/Field_START_23.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_24, "assets/Field_START_24.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_25, "assets/Field_START_25.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_26, "assets/Field_START_26.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_27, "assets/Field_START_27.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_28, "assets/Field_START_28.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_29, "assets/Field_START_29.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_30, "assets/Field_START_30.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_STARTSCREEN_31, "assets/Field_START_31.sprite", 39, 103, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	
	
	{ GRAPHICS_GAME_OVER_1, "assets/Game_over_1.sprite", 35, 169, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_GAME_OVER_2, "assets/Game_over_2.sprite", 35, 169, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_GAME_OVER_3, "assets/Game_over_3.sprite", 35, 169, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_GAME_OVER_4, "assets/Game_over_4.sprite", 35, 169, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_GAME_OVER_5, "assets/Game_over_5.sprite", 35, 169, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_GAME_OVER_6, "assets/Game_over_6.sprite", 35, 169, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_GAME_OVER_7, "assets/Game_over_7.sprite", 35, 169, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_GAME_OVER_8, "assets/Game_over_8.sprite", 35, 169, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_GAME_OVER_9, "assets/Game_over_9.sprite", 35, 169, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_GAME_OVER_10, "assets/Game_over_10.sprite", 35, 169, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_GAME_OVER_11, "assets/Game_over_11.sprite", 35, 169, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_GAME_OVER_12, "assets/Game_over_12.sprite", 35, 169, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_GAME_OVER_13, "assets/Game_over_13.sprite", 35, 169, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_GAME_OVER_14, "assets/Game_over_14.sprite", 35, 169, NULL, GRAPHICS_SPRITES_COLOR_WHITE }
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
	init_pair(GRAPHICS_SPRITES_COLOR_WHITE_FILLED, COLOR_WHITE, COLOR_WHITE);
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
	graphics_sprites_colors_t render_color = GRAPHICS_SPRITES_COLOR_WHITE;
	
	sprite = graphics_sprites_get(type);
	if(sprite == NULL || sprite->data == NULL)
	{
		return;
	}
	
	
	if(sprite->color != GRAPHICS_SPRITES_COLOR_RANDOM && sprite->color != GRAPHICS_SPRITES_COLOR_RANDOM_FILL)
	{
		render_color = sprite->color;
	}
	else if(sprite->color == GRAPHICS_SPRITES_COLOR_RANDOM_FILL)
	{
		// get random number in [2, 8]
		render_color = rand() % 7 + 2;
	}
	
	attron(COLOR_PAIR(render_color));
	
	for(y = 0; y < sprite->height; y++)
	{
		for(x = 0; x < sprite->width; x++)
		{
			// if transparency == 1 then render only if character of sprite != ' '
			if(transparency == 0 || (transparency == 1 && sprite->data[y * sprite->width + x] != ' '))
			{
				if(sprite->color == GRAPHICS_SPRITES_COLOR_RANDOM)
				{
					attron(COLOR_PAIR(rand() % 7 + 2));
				}
				
				mvaddch(render_y + y, render_x + x, sprite->data[y * sprite->width + x]);
				
				if(sprite->color == GRAPHICS_SPRITES_COLOR_RANDOM)
				{
					attroff(COLOR_PAIR(rand() % 7 + 2));
				}
			}
		}
	}
	
	attroff(COLOR_PAIR(render_color));
}

void graphics_sprites_render_box(int render_x, int render_y, int width, int height)
{
	int x = 0;
	int y = 0;
	
	attron(COLOR_PAIR(GRAPHICS_SPRITES_COLOR_WHITE_FILLED));
	
	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			mvaddch(render_y + y, render_x + x, '#');
		}
	}
	
	attroff(COLOR_PAIR(GRAPHICS_SPRITES_COLOR_WHITE_FILLED));
}

void graphics_sprites_screenshot(void)
{
	char path[80];
	time_t rawtime = 0;
	struct tm *timeinfo = NULL;
	FILE *file = NULL;
	int x = 0;
	int y = 0;
	
	memset(path, 0, 80 * sizeof(char));
	
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	
	strftime(path, 80, "screenshot-turbo-bomber-%Y-%m-%d-%H%M%S.txt", timeinfo);
	
	core_debug("Taking screenshot... %s", path);
	
	file = fopen(path, "w");
	if(file == NULL)
	{
		core_error("Failed to open screenshot file.");
		return;
	}
	
	for(y = 0; y < GRAPHICS_SPRITES_SCREENSHOT_HEIGHT; y++)
	{
		for(x = 0; x < GRAPHICS_SPRITES_SCREENSHOT_WIDTH; x++)
		{
			fputc(mvinch(y, x) & A_CHARTEXT, file);
		}
		
		fputc('\n', file);
	}
	
	fclose(file);
}
