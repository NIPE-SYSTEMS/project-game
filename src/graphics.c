#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#include "graphics.h"
#include "graphics-sprites.h"
#include "gameplay.h"

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
void graphics_render_sprite(int pos_x, int pos_y, graphics_fields_sprites_t index, char transparency)
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
	int field_index = 0;
	int render_x = 0;
	int render_y = 0;
	int hearts = 0;
	
	gameplay_field_t *gameplay_field = gameplay_get_field();
	gameplay_player_t *gameplay_player = gameplay_get_player();
	
	for(y = 0; y < GAMEPLAY_FIELD_HEIGHT; y++)
	{
		for(x = 0; x < GAMEPLAY_FIELD_WIDTH; x++)
		{
			field_index = y * GAMEPLAY_FIELD_WIDTH + x;
			render_x = (x * GRAPHICS_OFFSET_X) + GRAPHICS_OFFSET_X - GRAPHICS_SPRITE_WIDTH;
			render_y = (y * GRAPHICS_OFFSET_Y) + GRAPHICS_OFFSET_Y - GRAPHICS_SPRITE_HEIGHT;
			
			if(gameplay_field[field_index].bomb == 1) // bomb
			{
				graphics_render_sprite(render_x, render_y, BOMB_SPRITE, 0);
			}
			else if(gameplay_field[field_index].explosion == 1) // fire
			{
				graphics_render_sprite(render_x, render_y, EXPLOSION_SPRITE + graphics_spinning_animation_counter % 2, 0);
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
	mvprintw(GRAPHICS_HEALTH_Y, GRAPHICS_HEALTH_X, "Health:");
	for(hearts = 0; hearts < gameplay_player->health_points; hearts++)
	{
		graphics_render_sprite(GRAPHICS_HEALTH_X + (GRAPHICS_HEALTH_OFFSET_X * hearts), GRAPHICS_HEALTH_Y + GRAPHICS_HEALTH_OFFSET_Y, HEART_SPRITE, 0);
	}
	
	// player
	render_x = (gameplay_player->position_x * GRAPHICS_OFFSET_X) + GRAPHICS_OFFSET_X - GRAPHICS_SPRITE_WIDTH;
	render_y = (gameplay_player->position_y * GRAPHICS_OFFSET_Y) + GRAPHICS_OFFSET_Y - GRAPHICS_SPRITE_HEIGHT;
	
	if(gameplay_player->movement_cooldown > 1)
	{
		graphics_render_sprite(render_x, render_y, PLAYER_SPRITE, 1);
	}
	else
	{
		graphics_render_sprite(render_x, render_y, PLAYER_STANDING_SPRITE, 1);
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
