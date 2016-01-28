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

#include "gameplay-bombs.h"
#include "gameplay-players.h"
#include "core.h"
#include "ai-simulation.h"
#include "gameplay-items.h"
#include "gameplay.h"

static void gameplay_bombs_trigger_explosion(int position_x, int position_y);

gameplay_bombs_bomb_t *gameplay_bombs_bombs = NULL;

/**
 * This function adds a bomb to the bomb list.
 * 
 * @param player The player which has placed the bomb. (Owner of the bomb.)
 * @param position_x The x coordinate of the simulated bomb.
 * @param position_y The y coordinate of the simulated bomb.
 */
void gameplay_bombs_add(gameplay_players_player_t *player, int position_x, int position_y)
{
	gameplay_bombs_bomb_t *bomb = NULL;
	gameplay_bombs_bomb_t *current = NULL;
	
	bomb = malloc(sizeof(gameplay_bombs_bomb_t));
	if(bomb == NULL)
	{
		core_error("Failed to allocate bomb.");
		return;
	}
	
	bomb->position_x = position_x;
	bomb->position_y = position_y;
	bomb->explosion_timeout = GAMEPLAY_BOMBS_EXPLOSION_TIMEOUT;
	bomb->owner = player;
	bomb->next = NULL;
	
	// append to bombs list at end
	for(current = gameplay_bombs_bombs; current != NULL && current->next != NULL; current = current->next);
	
	if(current == NULL)
	{
		bomb->next = gameplay_bombs_bombs;
		gameplay_bombs_bombs = bomb;
	}
	else
	{
		current->next = bomb;
	}
	
	core_debug("Added bomb %p at (%i, %i)", gameplay_bombs_bombs, position_x, position_y);
}

/**
 * This function cleans up all bombs in the bomb list.
 */
void gameplay_bombs_cleanup(void)
{
	gameplay_bombs_bomb_t *current = NULL;
	gameplay_bombs_bomb_t *next_backup = NULL;
	
	core_debug("Cleanup bombs...");
	
	for(current = gameplay_bombs_bombs; current != NULL; current = next_backup)
	{
		next_backup = current->next;
		free(current);
	}
	
	gameplay_bombs_bombs = NULL;
}

/**
 * This function removes a bomb from the bomb list.
 * 
 * @param position_x The x coordinate of the bomb.
 * @param position_y The y coordinate of the bomb.
 */
void gameplay_bombs_remove(int position_x, int position_y)
{
	gameplay_bombs_bomb_t *current = NULL;
	gameplay_bombs_bomb_t *next_backup = NULL;
	
	for(current = gameplay_bombs_bombs; current != NULL; current = current->next)
	{
		if(current->position_x == position_x && current->position_y == position_y)
		{
			break;
		}
	}
	
	// cancel if nothing found
	if(current == NULL)
	{
		return;
	}
	
	// give the player the ability to place another bomb
	current->owner->placed_bombs--;
	
	// list start
	if(current == gameplay_bombs_bombs)
	{
		next_backup = current->next;
		free(current);
		gameplay_bombs_bombs = next_backup;
		return;
	}
	
	// rest of the list
	for(current = gameplay_bombs_bombs; current->next != NULL; current = current->next)
	{
		if(current->next->position_x == position_x && current->next->position_y == position_y)
		{
			next_backup = current->next->next;
			free(current->next);
			current->next = next_backup;
			break;
		}
	}
}

/**
 * This function handles the explosion of a bomb on the field.
 * 
 * @param position_x The x coordinate of the bomb.
 * @param position_y The y coordinate of the bomb.
 */
static void gameplay_bombs_explosion(int position_x, int position_y)
{
	int x = 0;
	int y = 0;
	
	gameplay_bombs_bomb_t *bomb = NULL;
	
	bomb = gameplay_bombs_get_bomb(position_x, position_y);
	if(bomb == NULL)
	{
		return;
	}
	
	for(x = position_x; x < GAMEPLAY_FIELD_WIDTH && x < position_x + bomb->owner->explosion_radius; x++)
	{
		gameplay_players_harm(x, position_y);
		gameplay_bombs_trigger_explosion(x, position_y);
		if(!gameplay_get_walkable(x, position_y, 1))
		{
			gameplay_destroy(x, position_y);
			gameplay_set_fire(x, position_y);
			break;
		}
		else if(gameplay_items_test_remove(x, position_y) == 1)
		{
			gameplay_set_fire(x, position_y);
			break;
		}
		gameplay_set_fire(x, position_y);
	}
	
	for(x = position_x - 1; x > 0 && x > position_x - bomb->owner->explosion_radius; x--)
	{
		gameplay_players_harm(x, position_y);
		gameplay_bombs_trigger_explosion(x, position_y);
		if(!gameplay_get_walkable(x, position_y, 1))
		{
			gameplay_destroy(x, position_y);
			gameplay_set_fire(x, position_y);
			break;
		}
		else if(gameplay_items_test_remove(x, position_y) == 1)
		{
			gameplay_set_fire(x, position_y);
			break;
		}
		gameplay_set_fire(x, position_y);
	}
	
	for(y = position_y; y < GAMEPLAY_FIELD_HEIGHT && y < position_y + bomb->owner->explosion_radius; y++)
	{
		gameplay_players_harm(position_x, y);
		gameplay_bombs_trigger_explosion(position_x, y);
		if(!gameplay_get_walkable(position_x, y, 1))
		{
			gameplay_destroy(position_x, y);
			gameplay_set_fire(position_x, y);
			break;
		}
		else if(gameplay_items_test_remove(position_x, y) == 1)
		{
			gameplay_set_fire(position_x, y);
			break;
		}
		gameplay_set_fire(position_x, y);
	}
	
	for(y = position_y - 1; y > 0 && y > position_y - bomb->owner->explosion_radius; y--)
	{
		gameplay_players_harm(position_x, y);
		gameplay_bombs_trigger_explosion(position_x, y);
		if(!gameplay_get_walkable(position_x, y, 1))
		{
			gameplay_destroy(position_x, y);
			gameplay_set_fire(position_x, y);
			break;
		}
		else if(gameplay_items_test_remove(position_x, y) == 1)
		{
			gameplay_set_fire(position_x, y);
			break;
		}
		gameplay_set_fire(position_x, y);
	}
}

/**
 * This function updates a bomb. (Timing, etc.)
 * 
 * @param bomb The bomb which should be updated.
 */
static void gameplay_bombs_bomb_update(gameplay_bombs_bomb_t *bomb)
{
	if(bomb->explosion_timeout == 0 )
	{
		gameplay_bombs_explosion(bomb->position_x, bomb->position_y);
		gameplay_bombs_remove(bomb->position_x, bomb->position_y);
		return;
	}
	
	if(bomb->explosion_timeout > 0)
	{
		bomb->explosion_timeout--;
	}
	
	ai_simulation_explosion(bomb->position_x, bomb->position_y, bomb->owner->explosion_radius, 0);
}

/**
 * This function updates a bomb. (Timing, etc.)
 */
void gameplay_bombs_update(void)
{
	gameplay_bombs_bomb_t *current = NULL;
	gameplay_bombs_bomb_t *next_backup = NULL;
	
	ai_simulation_reset();
	
	for(current = gameplay_bombs_bombs; current != NULL; current = next_backup)
	{
		next_backup = current->next;
		gameplay_bombs_bomb_update(current);
	}
	
	ai_simulation_copy_fire();
}

/**
 * This function returns the amount of bombs in the bomb list.
 * 
 * @return The amount of bombs.
 */
int gameplay_bombs_amount(void)
{
	gameplay_bombs_bomb_t *current = NULL;
	int amount = 0;
	
	for(current = gameplay_bombs_bombs; current != NULL; current = current->next)
	{
		amount++;
	}
	
	return amount;
}

/**
 * This function returns the bomb with the given index.
 * 
 * @param index The index of the bomb.
 * @return The selected bomb.
 */
gameplay_bombs_bomb_t *gameplay_bombs_get(int index)
{
	gameplay_bombs_bomb_t *current = NULL;
	int i = 0;
	
	for(current = gameplay_bombs_bombs; current != NULL; current = current->next)
	{
		if(index == i)
		{
			return current;
		}
		
		i++;
	}
	
	return NULL;
}

/**
 * This function returns if a bomb is placed at the given position.
 * 
 * @param position_x The x coordinate of the tile.
 * @param position_y The y coordinate of the tile.
 * @return 1 if a bomb is placed, 0 if not.
 */
int gameplay_bombs_get_bomb_placed(int position_x, int position_y)
{
	gameplay_bombs_bomb_t *current = NULL;
	
	for(current = gameplay_bombs_bombs; current != NULL; current = current->next)
	{
		if(current->position_x == position_x && current->position_y == position_y && current->explosion_timeout > 0)//current->fire_timeout > 0)
		{
			return 1;
		}
	}
	
	return 0;
}

/**
 * This function returns the bomb at the given position.
 * 
 * @param position_x The x coordinate of the tile.
 * @param position_y The y coordinate of the tile.
 * @return The bomb on the tile or NULL if no bomb is on the tile.
 */
gameplay_bombs_bomb_t *gameplay_bombs_get_bomb(int position_x, int position_y)
{
	gameplay_bombs_bomb_t *current = NULL;
	
	for(current = gameplay_bombs_bombs; current != NULL; current = current->next)
	{
		if(current->position_x == position_x && current->position_y == position_y)
		{
			return current;
		}
	}
	
	return NULL;
}

/**
 * This function triggers an explosion if a bomb is placed on the given tile.
 * 
 * @param position_x The x coordinate of the tile.
 * @param position_y The y coordinate of the tile.
 */
static void gameplay_bombs_trigger_explosion(int position_x, int position_y)
{
	if(gameplay_bombs_get_bomb_placed(position_x, position_y) == 1)
	{
		gameplay_bombs_bomb_t *bomb = gameplay_bombs_get_bomb(position_x, position_y);
		bomb->explosion_timeout = 0;
	}
}