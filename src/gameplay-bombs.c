#include <stdlib.h>

#include "gameplay-bombs.h"
#include "gameplay-players.h"
#include "core.h"

gameplay_bombs_bomb_t *gameplay_bombs_bombs = NULL;

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
	bomb->fire_timeout = GAMEPLAY_BOMBS_FIRE_TIMEOUT;
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

void gameplay_bombs_cleanup(void)
{
	gameplay_bombs_bomb_t *current = NULL;
	gameplay_bombs_bomb_t *next_backup = NULL;
	
	for(current = gameplay_bombs_bombs; current != NULL; current = next_backup)
	{
		next_backup = current->next;
		free(current);
	}
}

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

static void gameplay_bombs_explosion(int position_x, int position_y)
{
	int x = 0;
	int y = 0;
	
	for(x = position_x; x < GAMEPLAY_FIELD_WIDTH; x++)
	{
		gameplay_players_harm(x, position_y);
		
		if(!gameplay_get_walkable(x, position_y))
		{
			gameplay_destroy(x, position_y);
			break;
		}
	}
	
	for(x = position_x - 1; x > 0; x--)
	{
		gameplay_players_harm(x, position_y);
		
		if(!gameplay_get_walkable(x, position_y))
		{
			gameplay_destroy(x, position_y);
			break;
		}
	}
	
	for(y = position_y; y < GAMEPLAY_FIELD_HEIGHT; y++)
	{
		gameplay_players_harm(position_x, y);
		
		if(!gameplay_get_walkable(position_x, y))
		{
			gameplay_destroy(position_x, y);
			break;
		}
	}
	
	for(y = position_y - 1; y > 0; y--)
	{
		gameplay_players_harm(position_x, y);
		
		if(!gameplay_get_walkable(position_x, y))
		{
			gameplay_destroy(position_x, y);
			break;
		}
	}
}

static void gameplay_bombs_bomb_update(gameplay_bombs_bomb_t *bomb)
{
	if(bomb->explosion_timeout == 0 && bomb->fire_timeout == 0)
	{
		gameplay_bombs_remove(bomb->position_x, bomb->position_y);
	}
	
	if(bomb->explosion_timeout > 0)
	{
		bomb->explosion_timeout--;
	}
	
	if(bomb->explosion_timeout == 0 && bomb->fire_timeout == GAMEPLAY_BOMBS_FIRE_TIMEOUT)
	{
		gameplay_bombs_explosion(bomb->position_x, bomb->position_y);
	}
	
	if(bomb->explosion_timeout == 0 && bomb->fire_timeout > 0)
	{
		bomb->fire_timeout--;
	}
}

void gameplay_bombs_update(void)
{
	gameplay_bombs_bomb_t *current = NULL;
	gameplay_bombs_bomb_t *next_backup = NULL;
	
	for(current = gameplay_bombs_bombs; current != NULL; current = next_backup)
	{
		// core_debug("Found bomb: %p at (%i, %i)", current, current->position_x, current->position_y);
		
		next_backup = current->next;
		gameplay_bombs_bomb_update(current);
	}
}

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

int gameplay_bombs_get_bomb_placed(int position_x, int position_y)
{
	gameplay_bombs_bomb_t *current = NULL;
	
	for(current = gameplay_bombs_bombs; current != NULL; current = current->next)
	{
		if(current->position_x == position_x && current->position_y == position_y && current->explosion_timeout > 0 && current->fire_timeout > 0)
		{
			return 1;
		}
	}
	
	return 0;
}

static gameplay_bombs_bomb_t *gameplay_bombs_get_bomb(int position_x, int position_y)
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

int gameplay_bombs_get_fire(int position_x, int position_y)
{
	gameplay_bombs_bomb_t *bomb = NULL;
	int x = 0;
	int y = 0;
	
	for(x = position_x; x < GAMEPLAY_FIELD_WIDTH; x++)
	{
		if(!gameplay_get_walkable(x, position_y))
		{
			break;
		}
		
		bomb = gameplay_bombs_get_bomb(x, position_y);
		if(bomb == NULL)
		{
			continue;
		}
		
		if(bomb->explosion_timeout == 0 && bomb->fire_timeout > 0)
		{
			return 1;
		}
	}
	
	for(x = position_x; x > 0; x--)
	{
		if(!gameplay_get_walkable(x, position_y))
		{
			break;
		}
		
		bomb = gameplay_bombs_get_bomb(x, position_y);
		if(bomb == NULL)
		{
			continue;
		}
		
		if(bomb->explosion_timeout == 0 && bomb->fire_timeout > 0)
		{
			return 1;
		}
	}
	
	for(y = position_y; y < GAMEPLAY_FIELD_HEIGHT; y++)
	{
		if(!gameplay_get_walkable(position_x, y))
		{
			break;
		}
		
		bomb = gameplay_bombs_get_bomb(position_x, y);
		if(bomb == NULL)
		{
			continue;
		}
		
		if(bomb->explosion_timeout == 0 && bomb->fire_timeout > 0)
		{
			return 1;
		}
	}
	
	for(y = position_y; y > 0; y--)
	{
		if(!gameplay_get_walkable(position_x, y))
		{
			break;
		}
		
		bomb = gameplay_bombs_get_bomb(position_x, y);
		if(bomb == NULL)
		{
			continue;
		}
		
		if(bomb->explosion_timeout == 0 && bomb->fire_timeout > 0)
		{
			return 1;
		}
	}
	
	return 0;
}
