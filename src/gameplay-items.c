#include <stdlib.h>
#include <ncurses.h>

#include "gameplay.h"
#include "gameplay-items.h"
#include "gameplay-players.h"
#include "gameplay-bombs.h"
#include "core.h"

static gameplay_items_item_t *gameplay_items_items = NULL;

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
	
	core_debug("Added bomb %p at (%i, %i)", gameplay_items_items, position_x, position_y);
}


void gameplay_items_cleanup(void)
{
	gameplay_items_item_t *current = NULL;
	gameplay_items_item_t *next_backup = NULL;
	
	for(current = gameplay_items_items; current != NULL; current = next_backup)
	{
		next_backup = current->next;
		free(current);
	}
}

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

void gameplay_items_item_update()
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

int gameplay_items_item_placed(int position_x,int position_y)
{
	gameplay_items_item_t *current = NULL;
	for(current = gameplay_items_items; current != NULL; current = current->next)
	{
		if(current->position_x == position_x && current->position_y == position_y)
			return 1;
	}
	return 0;
}

gameplay_items_item_type_t gameplay_items_get_item_type (int position_x, int position_y)
{
	gameplay_items_item_t *current = NULL;
	for(current = gameplay_items_items; current != NULL; current = current->next)
	{
		if(current->position_x == position_x && current->position_y == position_y)
			return current->type;
	}
	return 0;
}

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