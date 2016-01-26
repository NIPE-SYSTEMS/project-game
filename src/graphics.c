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

static int set_show_debug = 0;
static int graphics_animation_counter = 0;
static int graphics_startscreen_counter = 0;
static int graphics_frames_for_startscreen = 60;
static int graphics_game_over_counter = 0;
static int graphics_game_over_checker = 0;

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
void graphics_startscreen(void)
{
	int i = 0;
	int b = 0;
	
	for(i = 0; i < 26; i++)
	{
		for(b = 0; b < 103; b++)
		{
			mvaddch(3+i, 3+b, displayed_text[i][b]);
		}
	}
	
	if(graphics_startscreen_counter <= 30)
	{
		for(i = 0; i < 26; i++)
		{
			for(b = 0; b < 103; b++)
			{
				if(startscreen_frames[graphics_startscreen_counter][i][b] != '#')
				{
					mvaddch(3+i, 3+b, startscreen_frames[graphics_startscreen_counter][i][b]);
				}
			}
		}
	}
	move(0, 0);
	refresh();
}

void graphics_game_over_function(void)
{
	int i = 0;
	int b = 0;
	
	for(i = 0; i < 35; i++)
	{
		for(b = 0; b < 69; b++)
		{
			if(graphics_game_over_counter <= 13)
			{
				mvaddch(3+i, 3+b, graphics_game_over[graphics_game_over_counter][i][b]);
			}
			else
			{
				if(graphics_game_over[13][i][b] == '#')
				{
					mvaddch(3+i, 3+b, graphics_game_over[13][i][b]);
				}
			}
		}
	}
	graphics_game_over_counter++;
	
	move(0, 0);
	refresh();
}

void graphics_render_players(void)
{
	// static char animation_blinking = 0;
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
		
		if(player->damage_cooldown > 0 && graphics_animation_counter%2 == 0)
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
				graphics_sprites_render(render_x, render_y, GRAPHICS_SPRITES_EXPLOSION_1 + graphics_animation_counter % 2, 0);
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
}

/**
 * This function handles everything of the rendering. It will render the user
 * interface by interpreting the field of the gameplay module.
 */
void graphics_main(void)
{
	gameplay_players_player_t *player = NULL;
	
	//I believe, that the Ai can already make decisions before the player gets to see the field. That is why the start screen should delay the entire game and not just the rendering.
	if(graphics_startscreen_counter >= graphics_frames_for_startscreen && graphics_game_over_checker == 0)
	{
		if(graphics_startscreen_counter >= (graphics_frames_for_startscreen + 1)) //Delayes the start of the AI by one frame
		{
			ai_core_enable();
		}
		else
		{
			graphics_startscreen_counter++;
		}
		
		player = gameplay_players_get_user();
		
		//Game Over scrren activation
		if(player->health_points == 0)
		{
			graphics_game_over_checker = 1;
		}
		
		//Debug Information
		
		if (set_show_debug)
		{
			graphics_render_debug();
		}
		else
		{
			graphics_render_information();
		}
		
		graphics_render_field();
		graphics_render_players();
		
		graphics_animation_counter++;
		move(0, 0);
		refresh();
	}
	else if(graphics_animation_counter == 0)
	{
		graphics_startscreen();
		graphics_startscreen_counter++;
	}
	else if(graphics_game_over_checker == 1)
	{
		//hier muss noch AI cleanup dazu!
		graphics_game_over_function();
	}
}

void graphics_render_breaked_game()
{
	if(set_show_debug)
	{
		graphics_render_debug();
	}
	else
	{
		graphics_render_information();
	}
	
	graphics_render_field();
	graphics_render_players();
	move(0, 0);
	refresh();
}