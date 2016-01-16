#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#include "graphics.h"
#include "graphics-sprites.h"
#include "gameplay.h"
#include "gameplay-players.h"
#include "gameplay-bombs.h"
#include "core.h"

static int graphics_spinning_animation_counter = 0;

/**
 * This function renders a sprite at a given position. It also can handle
 * transparency.
 * 
 * @param pos_x The X position.
 * @param pos_y The Y position.
 * @param index The index of the sprite.
 * @param transparency 1 to enable transparency rendering, 0 to disable it. 0
 *                     means that every new rendering will overwrite the
 *                     existing characters.
 */
void graphics_render_sprite(int pos_x, int pos_y, graphics_sprites_t index, char transparency)
{
	int x = 0;
	int y = 0;
	
	for(y = 0; y < GRAPHICS_SPRITE_HEIGHT; y++)
	{
		for(x = 0; x < GRAPHICS_SPRITE_WIDTH; x++)
		{
			// if transparency == 1 then render only if character of sprite != ' '
			if(transparency == 0 || (transparency == 1 && graphics_sprites[index][y][x] != ' '))
			{
				mvaddch(pos_y + y, pos_x + x, graphics_sprites[index][y][x]);
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
	int x = 0;
	int y = 0;
	int i = 0;
	int field_index = 0;
	int render_x = 0;
	int render_y = 0;
	// int hearts = 0;
	int player_amount = 0;
	gameplay_players_player_t *player = NULL;
	gameplay_field_t *gameplay_field = NULL;
	int bomb_amount = 0;
	gameplay_bombs_bomb_t *bomb = NULL;
	
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
				graphics_render_sprite(render_x, render_y, GRAPHICS_SPRITES_BOMB, 0);
			}
			else if(gameplay_bombs_get_fire(x, y) == 1) // fire
			{
				graphics_render_sprite(render_x, render_y, GRAPHICS_SPRITES_EXPLOSION + graphics_spinning_animation_counter % 2, 0);
			}
			else if(gameplay_field[field_index].item != 0) // item
			{
				graphics_render_sprite(render_x, render_y, gameplay_field[field_index].item, 0);
			}
			else // field
			{
				graphics_render_sprite(render_x, render_y, gameplay_field[field_index].type, 0);
			}
		}
	}
	
	// hearts
	// mvprintw(GRAPHICS_HEALTH_Y, GRAPHICS_HEALTH_X, "Health:");
	player = gameplay_players_get_user();
	// for(hearts = 0; hearts < player->health_points; hearts++)
	// {
	// 	graphics_render_sprite(GRAPHICS_HEALTH_X + (GRAPHICS_HEALTH_OFFSET_X * hearts), GRAPHICS_HEALTH_Y + GRAPHICS_HEALTH_OFFSET_Y, GRAPHICS_SPRITES_HEART, 0);
	// }
	
	// debug informations
	mvprintw(GRAPHICS_HEALTH_Y, GRAPHICS_HEALTH_X, "Debug informations:");
	mvprintw(GRAPHICS_HEALTH_Y + 1, GRAPHICS_HEALTH_X + 4, "Health: %i", player->health_points);
	mvprintw(GRAPHICS_HEALTH_Y + 2, GRAPHICS_HEALTH_X + 4, "Movement cooldown: %i", player->movement_cooldown);
	mvprintw(GRAPHICS_HEALTH_Y + 3, GRAPHICS_HEALTH_X + 4, "Movement cooldown initial: %i", player->movement_cooldown_initial);
	mvprintw(GRAPHICS_HEALTH_Y + 4, GRAPHICS_HEALTH_X + 4, "Position(x): %i", player->position_x);
	mvprintw(GRAPHICS_HEALTH_Y + 5, GRAPHICS_HEALTH_X + 4, "Position(y): %i", player->position_y);
	mvprintw(GRAPHICS_HEALTH_Y + 6, GRAPHICS_HEALTH_X + 4, "Placeable Bombs: %i", player->placeable_bombs);
	mvprintw(GRAPHICS_HEALTH_Y + 7, GRAPHICS_HEALTH_X + 4, "Placed Bombs: %i", player->placed_bombs);
	mvprintw(GRAPHICS_HEALTH_Y + 8, GRAPHICS_HEALTH_X + 4, "Item: %i", player->item);
	mvprintw(GRAPHICS_HEALTH_Y + 9, GRAPHICS_HEALTH_X + 4, "Item usage time: %i", player->item_usage_time);
	mvprintw(GRAPHICS_HEALTH_Y + 10, GRAPHICS_HEALTH_X + 4, "Damage cooldown: %i", player->damage_cooldown);
	
	bomb_amount = gameplay_bombs_amount();
	for(i = 0; i < bomb_amount; i++)
	{
		bomb = gameplay_bombs_get(i);
		if(bomb == NULL)
		{
			continue;
		}
		
		mvprintw(GRAPHICS_HEALTH_Y + 12 + i, GRAPHICS_HEALTH_X, "Bomb %i: %p at (%i, %i), %i, %i", i, bomb, bomb->position_x, bomb->position_y, bomb->explosion_timeout, bomb->fire_timeout);
	}
	
	// players
	player_amount = gameplay_players_amount();
	for(i = 0; i < player_amount; i++)
	{
		player = gameplay_players_get(i);
		if(player == NULL)
		{
			continue;
		}
		
		render_x = (player->position_x * GRAPHICS_OFFSET_X) + GRAPHICS_OFFSET_X - GRAPHICS_SPRITE_WIDTH;
		render_y = (player->position_y * GRAPHICS_OFFSET_Y) + GRAPHICS_OFFSET_Y - GRAPHICS_SPRITE_HEIGHT;
		
		if(player->movement_cooldown > 1)
		{
			graphics_render_sprite(render_x, render_y, GRAPHICS_SPRITES_PLAYER, 1);
		}
		else
		{
			graphics_render_sprite(render_x, render_y, GRAPHICS_SPRITES_PLAYER_STANDING, 1);
		}
	}
	
	// spinning animation
	mvaddch(GRAPHICS_SPINNING_ANIMATION_Y, GRAPHICS_SPINNING_ANIMATION_X, graphics_spinning_animation[graphics_spinning_animation_counter]);
	
	// mvprintw(20, 77, "Current item:");
	// if(gameplay_player->item == 0)
	// {
	// 	graphics_render_sprite(22,82, 2, 0);
	// }
	// else
	// {
	// 	graphics_render_sprite(22,82, gameplay_player->item, 0);
	// }
	
	move(0, 0);
	refresh();
	
	graphics_spinning_animation_counter++;
	graphics_spinning_animation_counter %= 4;
}
