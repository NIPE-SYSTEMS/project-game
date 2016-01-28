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

#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdio.h>

#include "graphics.h"
#include "graphics-sprites.h"
#include "gameplay.h"
#include "gameplay-players.h"
#include "gameplay-bombs.h"
#include "core.h"
#include "gameplay-items.h"
#include "ai-core.h"

void graphics_render_menu(void)
{
	static char keys_delay_counter = 0;
	static unsigned char keys_index = 0;
	static char animation_logo = 0;
	
	graphics_sprites_type_t keys[] =
	{
		GRAPHICS_SPRITES_TYPE_MENU_KEY_1,
		GRAPHICS_SPRITES_TYPE_MENU_KEY_2,
		GRAPHICS_SPRITES_TYPE_MENU_KEY_3,
		GRAPHICS_SPRITES_TYPE_MENU_KEY_4
	};
	
	graphics_sprites_render(GRAPHICS_MENU_LOGO_X, GRAPHICS_MENU_LOGO_Y, GRAPHICS_SPRITES_TYPE_MENU_LOGO_1 + animation_logo, 0);
	graphics_sprites_render(GRAPHICS_MENU_START_X, GRAPHICS_MENU_START_Y, GRAPHICS_SPRITES_TYPE_MENU_START, 0);
	mvprintw(GRAPHICS_MENU_KEYS_TEXT_Y, GRAPHICS_MENU_KEYS_TEXT_X, "HOW TO PLAY:");
	graphics_sprites_render(GRAPHICS_MENU_KEYS_X, GRAPHICS_MENU_KEYS_Y, keys[keys_index], 0);
	
	keys_delay_counter++;
	keys_delay_counter %= GRAPHICS_MENU_KEYS_DELAY;
	
	if(keys_delay_counter == 0)
	{
		keys_index++;
		keys_index %= sizeof(keys) / sizeof(keys[0]);
	}
	
	if(animation_logo == 0)
	{
		animation_logo = 1;
	}
	else
	{
		animation_logo = 0;
	}
}

void graphics_render_debug(void)
{
	int i = 0;
	int offset_line = 0;
	int player_amount = 0;
	gameplay_players_player_t *player = NULL;
	int bomb_amount = 0;
	gameplay_bombs_bomb_t *bomb = NULL;
	
	mvprintw(GRAPHICS_DEBUG_Y + offset_line++, GRAPHICS_DEBUG_X, "Players [");
	player_amount = gameplay_players_amount();
	for(i = 0; i < player_amount; i++)
	{
		player = gameplay_players_get(i);
		if(player == NULL)
		{
			continue;
		}
		
		mvprintw(GRAPHICS_DEBUG_Y + offset_line++, GRAPHICS_DEBUG_X + 2, "%s { h: %i, m: %i (%i), p: (%i, %i), b: %i (%i), e: %i, d: %i (%i) }", ((player->type == GAMEPLAY_PLAYERS_TYPE_AI)?("AI  "):("USER")), player->health_points, player->movement_cooldown, player->movement_cooldown_initial, player->position_x, player->position_y, player->placed_bombs, player->placeable_bombs, player->explosion_radius, player->damage_cooldown, player->damage_cooldown_initial);
	}
	mvprintw(GRAPHICS_DEBUG_Y + offset_line++, GRAPHICS_DEBUG_X, "]");
	
	offset_line++;
	
	mvprintw(GRAPHICS_DEBUG_Y + offset_line++, GRAPHICS_DEBUG_X, "Bombs [");
	bomb_amount = gameplay_bombs_amount();
	for(i = 0; i < bomb_amount; i++)
	{
		bomb = gameplay_bombs_get(i);
		if(bomb == NULL)
		{
			continue;
		}
		
		mvprintw(GRAPHICS_DEBUG_Y + offset_line++, GRAPHICS_DEBUG_X + 2, "{ o: %s, p: (%i, %i), e: %i }", ((bomb->owner->type == GAMEPLAY_PLAYERS_TYPE_AI)?("AI"):("USER")), bomb->position_x, bomb->position_y, bomb->explosion_timeout);
	}
	mvprintw(GRAPHICS_DEBUG_Y + offset_line++, GRAPHICS_DEBUG_X, "]");
}

/**
 * This function will render the startscreen which will
 * display the games logo and name.
 */
int graphics_render_start_screen(void)
{
	static int animation_counter = 0;
	
	if(animation_counter > 60)
	{
		return 0;
	}
	
	graphics_sprites_render(0, 0, GRAPHICS_SPRITES_TYPE_START_SCREEN_TEXT, 0);
	
	if(animation_counter <= 30)
	{
		graphics_sprites_render(0, 0, GRAPHICS_SPRITES_TYPE_START_SCREEN_1 + animation_counter, 2);
	}
	
	animation_counter++;
	
	return 1;
}

void graphics_render_game_over_screen(void)
{
	static int animation_counter = 0;
	
	if(animation_counter <= 13)
	{
		graphics_sprites_render(0, 0, GRAPHICS_SPRITES_TYPE_GAME_OVER_SCREEN_1 + animation_counter, 0);
	}
	else
	{
		graphics_sprites_render(0, 0, GRAPHICS_SPRITES_TYPE_GAME_OVER_SCREEN_14, 0);
	}
	
	if(animation_counter <= 13)
	{
		animation_counter++;
	}
}

void graphics_win_screen(void)
{
	// static int graphics_win_counter = 0;
	
	// char *current_frame = NULL;
	
	// graphics_sprites_type_t /*GRAPHICS_GAME_OVER_1*/ = current_frame; ///Not defined jet
	
	// current_frame += graphics_win_counter;
	
	// if(graphics_win_counter <= 13)
	// {
	// 	graphics_sprites_render(0, 0, current_frame, 0);
	// }
	// else
	// {
	// 	graphics_sprites_render(0, 0, graphics_sprites_sprites[/*GRAPHICS_GAME_OVER_14*/].data, 0); //Not defined jet
	// }
	
	
	// if(graphics_win_counter <= /*13*/) //Not defined jet
	// {
	// 	graphics_win_counter++;
	// }
}


void graphics_render_players(void)
{
	static char animation_blinking = 0;
	gameplay_players_player_t *player = NULL;
	int player_amount = 0;
	int render_x = 0;
	int render_y = 0;
	int i = 0;
	graphics_sprites_type_t player_sprite = GRAPHICS_SPRITES_TYPE_PLAYER;
	graphics_sprites_type_t player_sprite_standing = GRAPHICS_SPRITES_TYPE_PLAYER_STANDING;
	
	player = gameplay_players_get_user();
	if(player == NULL)
	{
		return;
	}
	
	if(player->turbo_mode_activated == 1)
	{
		player_sprite = GRAPHICS_SPRITES_TYPE_PLAYER_TURBO_MODE;
		player_sprite_standing = GRAPHICS_SPRITES_TYPE_PLAYER_STANDING_TURBO_MODE;
	}
	
	player_amount = gameplay_players_amount();
	for(i = 0; i < player_amount; i++)
	{
		player = gameplay_players_get(i);
		if(player == NULL)
		{
			continue;
		}
		
		if(player->damage_cooldown > 0 && animation_blinking == 0)
		{
			render_x = (player->position_x * GRAPHICS_OFFSET_X) + GRAPHICS_OFFSET_X - GRAPHICS_SPRITE_WIDTH;
			render_y = (player->position_y * GRAPHICS_OFFSET_Y) + GRAPHICS_OFFSET_Y - GRAPHICS_SPRITE_HEIGHT;
			
			if(player->movement_cooldown > 1)
			{
				if(player->type == GAMEPLAY_PLAYERS_TYPE_USER)
				{
					graphics_sprites_render(render_x, render_y, player_sprite, 1);
				}
				else
				{
					graphics_sprites_render(render_x, render_y, GRAPHICS_SPRITES_ENEMY, 1);
				}
				
				}
			else
			{
				if(player->type == GAMEPLAY_PLAYERS_TYPE_USER)
				{
					graphics_sprites_render(render_x, render_y, player_sprite_standing, 1);
				}
				else
				{
					graphics_sprites_render(render_x, render_y, GRAPHICS_SPRITES_ENEMY_STANDING, 1);
				}
			}
		}
		else if(player->damage_cooldown == 0)
		{
			render_x = (player->position_x * GRAPHICS_OFFSET_X) + GRAPHICS_OFFSET_X - GRAPHICS_SPRITE_WIDTH;
			render_y = (player->position_y * GRAPHICS_OFFSET_Y) + GRAPHICS_OFFSET_Y - GRAPHICS_SPRITE_HEIGHT;
			
			if(player->movement_cooldown > 1)
			{
				if(player->type == GAMEPLAY_PLAYERS_TYPE_USER)
				{
					graphics_sprites_render(render_x, render_y, player_sprite, 1);
				}
				else
				{
					graphics_sprites_render(render_x, render_y, GRAPHICS_SPRITES_ENEMY, 1);
				}
				
				}
			else
			{
				if(player->type == GAMEPLAY_PLAYERS_TYPE_USER)
				{
					graphics_sprites_render(render_x, render_y, player_sprite_standing, 1);
				}
				else
				{
					graphics_sprites_render(render_x, render_y, GRAPHICS_SPRITES_ENEMY_STANDING, 1);
				}
			}
		}
	}
	
	if(animation_blinking == 0)
	{
		animation_blinking = 1;
	}
	else
	{
		animation_blinking = 0;
	}
}

void graphics_render_information(void)
{
	int i = 0;
	gameplay_players_player_t *player = NULL;
	
	player = gameplay_players_get_user();
	
	// hearts
	mvprintw(GRAPHICS_HEALTH_Y, GRAPHICS_HEALTH_X, "Health:");
	
	for(i = 0; i < player->health_points; i++)
	{
		graphics_sprites_render(GRAPHICS_HEALTH_X + (GRAPHICS_HEALTH_OFFSET_X * i), GRAPHICS_HEALTH_Y + GRAPHICS_HEALTH_OFFSET_Y, GRAPHICS_SPRITES_HEART, 0);
	}
	
	// bombs
	mvprintw(GRAPHICS_HEALTH_Y+7, GRAPHICS_HEALTH_X, "Placed Bombs:");
	
	for(i = 0; i < player->placeable_bombs; i++)
	{
		graphics_sprites_render(GRAPHICS_HEALTH_X + (GRAPHICS_HEALTH_OFFSET_X * i), GRAPHICS_HEALTH_Y + GRAPHICS_HEALTH_OFFSET_Y+7, GRAPHICS_SPRITES_BOMBS_PLACABLE, 0);
	}
	
	for(i = 0; i < player->placed_bombs; i++)
	{
		graphics_sprites_render(GRAPHICS_HEALTH_X + (GRAPHICS_HEALTH_OFFSET_X * i), GRAPHICS_HEALTH_Y + GRAPHICS_HEALTH_OFFSET_Y+7, GRAPHICS_SPRITES_BOMB, 0);
	}
	
	// speed
	mvprintw(GRAPHICS_HEALTH_Y+14, GRAPHICS_HEALTH_X, "Speed:");
	
	for(i = 0; i < (6-player->movement_cooldown_initial); i++)
	{
		graphics_sprites_render(GRAPHICS_HEALTH_X + (GRAPHICS_HEALTH_OFFSET_X * i), GRAPHICS_HEALTH_Y + GRAPHICS_HEALTH_OFFSET_Y+14, GRAPHICS_SPRITES_SPEED, 0);
	}
	
	// blast radius
	mvprintw(GRAPHICS_HEALTH_Y+21, GRAPHICS_HEALTH_X, "Blast radius:");
	
	for(i = 0; i < player->explosion_radius; i++)
	{
		graphics_sprites_render(GRAPHICS_HEALTH_X + (GRAPHICS_HEALTH_OFFSET_X * i), GRAPHICS_HEALTH_Y + GRAPHICS_HEALTH_OFFSET_Y+21, GRAPHICS_SPRITES_EXPLOSION_1, 0);
	}
}

void graphics_render_field(void)
{
	static char animation_blinking = 0;
	int x = 0;
	int y = 0;
	int field_index = 0;
	int render_x = 0;
	int render_y = 0;
	gameplay_field_t *gameplay_field = NULL;
	
	gameplay_field = gameplay_get_field();
	
	for(y = 0; y < GAMEPLAY_FIELD_HEIGHT; y++)
	{
		for(x = 0; x < GAMEPLAY_FIELD_WIDTH; x++)
		{
			field_index = y * GAMEPLAY_FIELD_WIDTH + x;
			render_x = (x * GRAPHICS_OFFSET_X) + GRAPHICS_OFFSET_X - GRAPHICS_SPRITE_WIDTH;
			render_y = (y * GRAPHICS_OFFSET_Y) + GRAPHICS_OFFSET_Y - GRAPHICS_SPRITE_HEIGHT;
			
			if(gameplay_bombs_get_bomb_placed(x, y) == 1) // bomb
			{
				graphics_sprites_render(render_x, render_y, GRAPHICS_SPRITES_BOMB, 0);
			}
			else if(gameplay_get_fire(x, y) == 1) // fire
			{
				graphics_sprites_render(render_x, render_y, GRAPHICS_SPRITES_EXPLOSION_1 + animation_blinking, 0);
			}
			else if(gameplay_items_item_placed(x, y) != 0) // item
			{
				graphics_sprites_render(render_x, render_y, gameplay_items_get_item_type(x, y), 0);
			}
			else // field
			{
				graphics_sprites_render(render_x, render_y, gameplay_field[field_index].type, 0);
			}
		}
	}
	
	if(animation_blinking == 0)
	{
		animation_blinking = 1;
	}
	else
	{
		animation_blinking = 0;
	}
}

void graphics_render_qr_code(void)
{
	char qr_code[] =
	{
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,
		1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1,
		1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1,
		1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1,
		1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
		1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1,
		1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1,
		1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1,
		1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1,
		1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1,
		1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1,
		1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1,
		1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1,
		1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1,
		1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1,
		1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1,
		1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
		1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1,
		1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
		1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	};
	int x = 0;
	int y = 0;
	int render_x = 0;
	int render_y = 0;
	int offset_line = 0;
	
	for(y = 0; y < GRAPHICS_QR_CODE_SIZE; y++)
	{
		for(x = 0; x < GRAPHICS_QR_CODE_SIZE; x++)
		{
			if(qr_code[y * GRAPHICS_QR_CODE_SIZE + x] == 1)
			{
				render_x = (x * GRAPHICS_QR_CODE_PIXEL_SIZE_WIDTH) + GRAPHICS_QR_CODE_OFFSET_X;
				render_y = (y * GRAPHICS_QR_CODE_PIXEL_SIZE_HEIGHT) + GRAPHICS_QR_CODE_OFFSET_Y;
				
				graphics_sprites_render_box(render_x, render_y, GRAPHICS_QR_CODE_PIXEL_SIZE_WIDTH, GRAPHICS_QR_CODE_PIXEL_SIZE_HEIGHT);
			}
		}
	}
	
	render_x = GRAPHICS_QR_CODE_OFFSET_X + GRAPHICS_QR_CODE_PIXEL_SIZE_WIDTH * GRAPHICS_QR_CODE_SIZE / 2 - 44 / 2;
	render_y = GRAPHICS_QR_CODE_OFFSET_Y + GRAPHICS_QR_CODE_PIXEL_SIZE_HEIGHT * GRAPHICS_QR_CODE_SIZE + 2;
	mvprintw(render_y, render_x, "https://github.com/NIPE-SYSTEMS/turbo-bomber");
	
	render_x = GRAPHICS_QR_CODE_OFFSET_X + GRAPHICS_QR_CODE_PIXEL_SIZE_WIDTH * GRAPHICS_QR_CODE_SIZE / 2 - 12 / 2;
	render_y = GRAPHICS_QR_CODE_OFFSET_Y + GRAPHICS_QR_CODE_PIXEL_SIZE_HEIGHT * GRAPHICS_QR_CODE_SIZE + 4;
	mvprintw(render_y, render_x, "Made in 2016");
	
	render_x = 2 * GRAPHICS_QR_CODE_OFFSET_X + GRAPHICS_QR_CODE_PIXEL_SIZE_WIDTH * GRAPHICS_QR_CODE_SIZE;
	render_y = GRAPHICS_QR_CODE_OFFSET_Y;
	mvprintw(render_y, render_x, "Features:");
	
	offset_line = 2;
	
	mvprintw(render_y + offset_line++, render_x, " - Command line game (based on ncurses)");
	mvprintw(render_y + offset_line++, render_x, " - Animations");
	mvprintw(render_y + offset_line++, render_x, " - Complex cutscenes (made by hand)");
	mvprintw(render_y + offset_line++, render_x, " - Colors (more or less)");
	mvprintw(render_y + offset_line++, render_x, " - ASCII art");
	mvprintw(render_y + offset_line++, render_x, " - Cheats (TURBO-MODE)");
	mvprintw(render_y + offset_line++, render_x, " - Sprite rendering (based on ncurses)");
	mvprintw(render_y + offset_line++, render_x, " - Dynamic sprite reading (file I/O)");
	mvprintw(render_y + offset_line++, render_x, " - Multilayered rendering (alpha rendering)");
	mvprintw(render_y + offset_line++, render_x, " - AI: Bomb spot selection");
	mvprintw(render_y + offset_line++, render_x, " - AI: Explosion simulation (array magic)");
	mvprintw(render_y + offset_line++, render_x, " - AI: Pathfinding (wavefront/floodfill)");
	mvprintw(render_y + offset_line++, render_x, " - AI: Job lists (linked lists)");
	mvprintw(render_y + offset_line++, render_x, " - Explosion algorithm (array magic v2)");
	mvprintw(render_y + offset_line++, render_x, " - Explosion triggers nearby bombs");
	mvprintw(render_y + offset_line++, render_x, " - Items, power ups (linked lists, structs)");
	mvprintw(render_y + offset_line++, render_x, " - Destroyable items");
	mvprintw(render_y + offset_line++, render_x, " - Destructable walls");
	mvprintw(render_y + offset_line++, render_x, " - Random drops (item drops, probabilities)");
	mvprintw(render_y + offset_line++, render_x, " - Game states (enums, controlled state changes)");
	mvprintw(render_y + offset_line++, render_x, " - Logging, debug output (file I/O)");
	mvprintw(render_y + offset_line++, render_x, " - Git, public repository with complete code");
	mvprintw(render_y + offset_line++, render_x, " - Modular code (many header files)");
	mvprintw(render_y + offset_line++, render_x, " - Game loop (timing)");
	mvprintw(render_y + offset_line++, render_x, " - Keyboard input handling (based on ncurses)");
	mvprintw(render_y + offset_line++, render_x, " - QR code (maybe this one)");
	mvprintw(render_y + offset_line++, render_x, " - Game pausing");
	mvprintw(render_y + offset_line++, render_x, " - Screenshot taking (file I/O)");
}
