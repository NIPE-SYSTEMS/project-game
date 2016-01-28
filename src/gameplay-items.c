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
#include <ncurses.h>

#include "gameplay.h"
#include "gameplay-items.h"
#include "gameplay-players.h"
#include "gameplay-bombs.h"
#include "core.h"

static gameplay_items_item_t *gameplay_items_items = NULL;

/**
 * This function adds an item to the item list.
 * 
 * @param type The type of the array.
 * @param position_x The x coordinate of the new item.
 * @param position_y The y coordinate of the new item.
 */
void gameplay_items_add_item(gameplay_items_item_type_t type, int position_x, int position_y)
{
	gameplay_items_item_t *item = NULL;
	gameplay_items_item_t *current = NULL;
	
	item = malloc(sizeof(gameplay_items_item_t));
	if(item == NULL)
	{
		core_error("Failed to allocate item.");
		return;
	}
	item->type = type;
	item->position_x = position_x;
	item->position_y = position_y;
	item->despawn_timer = GAMEPLAY_ITEMS_ITEM_DESPAWN;
	item->next = NULL;
	
	// append to bombs list at end
	for(current = gameplay_items_items; current != NULL && current->next != NULL; current = current->next);
	
	if(current == NULL)
	{
		item->next = gameplay_items_items;
		gameplay_items_items = item;
	}
	else
	{
		current->next = item;
	}
	
	core_debug("Added item %p at (%i, %i)", gameplay_items_items, position_x, position_y);
}

/**
 * This function cleans up all items in the item list.
 */
void gameplay_items_cleanup(void)
{
	gameplay_items_item_t *current = NULL;
	gameplay_items_item_t *next_backup = NULL;
	
	core_debug("Cleanup items...");
	
	for(current = gameplay_items_items; current != NULL; current = next_backup)
	{
		next_backup = current->next;
		free(current);
	}
	
	gameplay_items_items = NULL;
}

/**
 * This function removes a item from the item list.
 * 
 * @param position_x The x coordinate of the item.
 * @param position_y The y coordinate of the item.
 */
void gameplay_items_remove(int position_x, int position_y)
{
	gameplay_items_item_t *current = NULL;
	gameplay_items_item_t *next_backup = NULL;
	
	for(current = gameplay_items_items; current != NULL; current = current->next)
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
	
	// list start
	if(current == gameplay_items_items)
	{
		next_backup = current->next;
		free(current);
		gameplay_items_items = next_backup;
		return;
	}
	
	// rest of the list
	for(current = gameplay_items_items; current->next != NULL; current = current->next)
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
 * This function updates all items. (Timing, etc.)
 */
void gameplay_items_item_update(void)
{
	gameplay_items_item_t *current = NULL;
	
	for(current = gameplay_items_items; current != NULL; current = current->next)
	{
		current->despawn_timer--;
		if(current->despawn_timer == 0)
		{
			gameplay_items_remove(current->position_x, current->position_y);
		
		}
	}
}

/**
 * This function returns if a item is placed at the given position.
 * 
 * @param position_x The x coordinate of the tile.
 * @param position_y The y coordinate of the tile.
 * @return 1 if a item is placed, 0 if not.
 */
int gameplay_items_item_placed(int position_x,int position_y)
{
	gameplay_items_item_t *current = NULL;
	for(current = gameplay_items_items; current != NULL; current = current->next)
	{
		if(current->position_x == position_x && current->position_y == position_y)
		{
			return 1;
		}
	}
	
	return 0;
}

/**
 * This function returns the item type at the given position.
 * 
 * @param position_x The x coordinate of the tile.
 * @param position_y The y coordinate of the tile.
 * @return The item type on the tile or 0 if no item is on the tile.
 */
gameplay_items_item_type_t gameplay_items_get_item_type(int position_x, int position_y)
{
	gameplay_items_item_t *current = NULL;
	for(current = gameplay_items_items; current != NULL; current = current->next)
	{
		if(current->position_x == position_x && current->position_y == position_y)
		{
			return current->type;
		}
	}
	
	return 0;
}

/**
 * This function removes a item from the item list and returns if it was
 * successful.
 * 
 * @param position_x The x coordinate of the item.
 * @param position_y The y coordinate of the item.
 * @return 1 on success (removal), else 0
 */
int gameplay_items_test_remove(int position_x, int position_y)
{
	if(gameplay_items_item_placed(position_x, position_y) == 1)
	{
		gameplay_items_remove(position_x, position_y);
		return 1;
	}
	else
	{
		return 0;
	}
}