/*
 * Copyright (C) 2015 NIPE-SYSTEMS
 * Copyright (C) 2015 Jonas Krug
 * Copyright (C) 2015 Tim Gevers
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
	
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_TEXT, "assets/start_screen_text.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_1, "assets/start_screen_1.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_2, "assets/start_screen_2.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_3, "assets/start_screen_3.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_4, "assets/start_screen_4.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_5, "assets/start_screen_5.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_6, "assets/start_screen_6.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_7, "assets/start_screen_7.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_8, "assets/start_screen_8.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_9, "assets/start_screen_9.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_10, "assets/start_screen_10.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_11, "assets/start_screen_11.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_12, "assets/start_screen_12.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_13, "assets/start_screen_13.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_14, "assets/start_screen_14.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_15, "assets/start_screen_15.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_16, "assets/start_screen_16.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_17, "assets/start_screen_17.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_18, "assets/start_screen_18.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_19, "assets/start_screen_19.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_20, "assets/start_screen_20.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_21, "assets/start_screen_21.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_22, "assets/start_screen_22.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_23, "assets/start_screen_23.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_24, "assets/start_screen_24.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_25, "assets/start_screen_25.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_26, "assets/start_screen_26.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_27, "assets/start_screen_27.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_28, "assets/start_screen_28.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_29, "assets/start_screen_29.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_30, "assets/start_screen_30.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_START_SCREEN_31, "assets/start_screen_31.sprite", 103, 26, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	
	{ GRAPHICS_SPRITES_TYPE_GAME_OVER_SCREEN_1, "assets/game_over_screen_1.sprite", 69, 35, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_GAME_OVER_SCREEN_2, "assets/game_over_screen_2.sprite", 69, 35, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_GAME_OVER_SCREEN_3, "assets/game_over_screen_3.sprite", 69, 35, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_GAME_OVER_SCREEN_4, "assets/game_over_screen_4.sprite", 69, 35, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_GAME_OVER_SCREEN_5, "assets/game_over_screen_5.sprite", 69, 35, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_GAME_OVER_SCREEN_6, "assets/game_over_screen_6.sprite", 69, 35, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_GAME_OVER_SCREEN_7, "assets/game_over_screen_7.sprite", 69, 35, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_GAME_OVER_SCREEN_8, "assets/game_over_screen_8.sprite", 69, 35, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_GAME_OVER_SCREEN_9, "assets/game_over_screen_9.sprite", 69, 35, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_GAME_OVER_SCREEN_10, "assets/game_over_screen_10.sprite", 69, 35, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_GAME_OVER_SCREEN_11, "assets/game_over_screen_11.sprite", 69, 35, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_GAME_OVER_SCREEN_12, "assets/game_over_screen_12.sprite", 69, 35, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_GAME_OVER_SCREEN_13, "assets/game_over_screen_13.sprite", 69, 35, NULL, GRAPHICS_SPRITES_COLOR_WHITE },
	{ GRAPHICS_SPRITES_TYPE_GAME_OVER_SCREEN_14, "assets/game_over_screen_14.sprite", 69, 35, NULL, GRAPHICS_SPRITES_COLOR_WHITE }
};

/**
 * This function initializes the colors.
 */
void graphics_sprites_init_colors(void)
{
	core_debug("Initializing colors...");
	
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

/**
 * This function reads a sprite file of a given size and returns the data.
 * 
 * @param path The path of the sprite file.
 * @param width The width of the sprite.
 * @param height The height of the sprite.
 * @return The read data.
 */
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

/**
 * This function initializes all sprites.
 */
void graphics_sprites_init(void)
{
	int i = 0;
	
	core_debug("Loading sprites...");
	
	graphics_sprites_init_colors();
	
	for(i = 0; i < (int)(sizeof(graphics_sprites_sprites) / sizeof(graphics_sprites_sprites[0])); i++)
	{
		graphics_sprites_sprites[i].data = graphics_sprites_read(graphics_sprites_sprites[i].path, graphics_sprites_sprites[i].width, graphics_sprites_sprites[i].height);
	}
}

/**
 * This function cleans up all sprites.
 */
void graphics_sprites_cleanup(void)
{
	int i = 0;
	
	for(i = 0; i < (int)(sizeof(graphics_sprites_sprites) / sizeof(graphics_sprites_sprites[0])); i++)
	{
		free(graphics_sprites_sprites[i].data);
	}
}

/**
 * This function returns the sprite of a given type.
 * 
 * @param type The type of the sprite.
 * @return The sprite informations.
 */
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

/**
 * This function renders a sprite of a given type at a given position. It is
 * possible to render sprites with transparency.
 * 
 * @param render_x The x coordinate of the position on the screen.
 * @param render_y The y coordinate of the position on the screen.
 * @param type The type of the sprite.
 * @param transparency 0 means no transparency, 1 means transparency where ' '
 *                     are transparent, 2 means transparency where '#' are
 *                     transparent.
 */
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
			if(transparency == 0 || (transparency == 1 && sprite->data[y * sprite->width + x] != ' ') || (transparency == 2 && sprite->data[y * sprite->width + x] != '#'))
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

/**
 * This function renders a colored (white) box at a given position with a given
 * size.
 * 
 * @param render_x The x coordinate of the position on the screen.
 * @param render_y The y coordinate of the position on the screen.
 * @param width The width of the box.
 * @param height The height of the box.
 */
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

/**
 * This function takes a screenshot and saves it to a text file. The filename
 * includes the current date.
 */
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
