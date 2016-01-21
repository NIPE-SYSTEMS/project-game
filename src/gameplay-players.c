#include <stdlib.h>

#include "gameplay-players.h"
#include "gameplay-bombs.h"
#include "gameplay.h"
#include "core.h"
#include "ai-core.h"

gameplay_players_player_t *gameplay_players_players = NULL;

void gameplay_players_add(int position_x, int position_y, gameplay_players_type_t type)
{
	gameplay_players_player_t *player = NULL;
	
	player = malloc(sizeof(gameplay_players_player_t));
	if(player == NULL)
	{
		core_error("Failed to allocate player.");
		return;
	}
	
	player->health_points = 3;
	player->movement_cooldown = 0;
	player->movement_cooldown_initial = GAMEPLAY_PLAYERS_MOVEMENT_COOLDOWN;
	player->position_x = position_x;
	player->position_y = position_y;
	player->placeable_bombs = 3;
	player->placed_bombs = 0;
	player->item = EMPTY;
	player->item_usage_time = 0;
	player->damage_cooldown = 0;
	player->damage_cooldown_initial = GAMEPLAY_PLAYERS_DAMAGE_COOLDOWN;
	player->type = type;
	player->jobs = NULL;
	player->next = NULL;
	
	// append to player list
	player->next = gameplay_players_players;
	gameplay_players_players = player;
	
	core_debug("Added player %p at (%i, %i)", gameplay_players_players, position_x, position_y);
}

void gameplay_players_cleanup(void)
{
	gameplay_players_player_t *current = NULL;
	gameplay_players_player_t *next_backup = NULL;
	
	for(current = gameplay_players_players; current != NULL; current = next_backup)
	{
		next_backup = current->next;
		ai_core_cleanup(current);
		free(current);
	}
}

void gameplay_players_remove(int position_x, int position_y)
{
	gameplay_players_player_t *current = NULL;
	gameplay_players_player_t *next_backup = NULL;
	
	for(current = gameplay_players_players; current != NULL; current = current->next)
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
	if(current == gameplay_players_players)
	{
		next_backup = current->next;
		free(current);
		gameplay_players_players = next_backup;
		return;
	}
	
	// rest of the list
	for(current = gameplay_players_players; current->next != NULL; current = current->next)
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

static void gameplay_players_player_update(gameplay_players_player_t *player)
{
	if(player->movement_cooldown > 0)
	{
		player->movement_cooldown--;
	}
	
	if(player->item_usage_time > 0)
	{
		player->item_usage_time--;
		
		if(player->item_usage_time == 0)
		{
			player->item = EMPTY;
		}
	}
	
	if(player->damage_cooldown > 0)
	{
		player->damage_cooldown--;
	}
}

void gameplay_players_update(void)
{
	gameplay_players_player_t *current = NULL;
	
	for(current = gameplay_players_players; current != NULL; current = current->next)
	{
		gameplay_players_player_update(current);
	}
}

gameplay_players_player_t *gameplay_players_get_user(void)
{
	gameplay_players_player_t *current = NULL;
	
	for(current = gameplay_players_players; current != NULL; current = current->next)
	{
		if(current->type == GAMEPLAY_PLAYERS_TYPE_USER)
		{
			return current;
		}
	}
	
	return NULL;
}

void gameplay_players_move(gameplay_players_direction_t direction)
{
	gameplay_players_player_t *player = NULL;
	int position_target_x = 0;
	int position_target_y = 0;
	
	player = gameplay_players_get_user();
	if(player == NULL)
	{
		core_error("Failed to find user controlled player.");
		return;
	}
	
	// calculate positions for testing
	position_target_x = player->position_x;
	position_target_y = player->position_y;
	
	switch(direction)
	{
		case GAMEPLAY_PLAYERS_DIRECTION_UP:
		{
			position_target_y--;
			break;
		}
		case GAMEPLAY_PLAYERS_DIRECTION_RIGHT:
		{
			position_target_x++;
			break;
		}
		case GAMEPLAY_PLAYERS_DIRECTION_DOWN:
		{
			position_target_y++;
			break;
		}
		case GAMEPLAY_PLAYERS_DIRECTION_LEFT:
		{
			position_target_x--;
			break;
		}
	}
	
	// test if a bomb is placed on target tile
	if(gameplay_bombs_get_bomb_placed(position_target_x, position_target_y))
	{
		return;
	}
	
	// test movement cooldown
	if(player->movement_cooldown > 0)
	{
		return;
	}
	
	// test walkability
	if(!gameplay_get_walkable(position_target_x, position_target_y))
	{
		return;
	}
	
	// at this point all dependencies for a movement are fulfilled
	// now actually move the player
	core_debug("Moving to (%i, %i)", position_target_x, position_target_y);
	player->position_x = position_target_x;
	player->position_y = position_target_y;
	player->movement_cooldown = player->movement_cooldown_initial;
}

int gameplay_players_amount(void)
{
	gameplay_players_player_t *current = NULL;
	int amount = 0;
	
	for(current = gameplay_players_players; current != NULL; current = current->next)
	{
		amount++;
	}
	
	return amount;
}

gameplay_players_player_t *gameplay_players_get(int index)
{
	gameplay_players_player_t *current = NULL;
	int i = 0;
	
	for(current = gameplay_players_players; current != NULL; current = current->next)
	{
		if(index == i)
		{
			return current;
		}
		
		i++;
	}
	
	return NULL;
}

int gameplay_player_get_player(int position_x, int position_y)
{
	gameplay_players_player_t *current = NULL;
	
	for(current = gameplay_players_players; current != NULL; current = current->next)
	{
		if(current->position_x == position_x && current->position_y == position_y)
		{
			return 1;
		}
	}
	
	return 0;
}

void gameplay_players_place_bomb(void)
{
	gameplay_players_player_t *player = NULL;
	
	player = gameplay_players_get_user();
	if(player == NULL)
	{
		core_error("Failed to find user controlled player.");
		return;
	}
	
	// test if player has bombs available
	if(player->placeable_bombs - player->placed_bombs == 0)
	{
		return;
	}
	
	// only place one bomb on a tile
	if(gameplay_bombs_get_bomb_placed(player->position_x, player->position_y) == 1)
	{
		return;
	}
	
	core_debug("Placing bomb at (%i, %i)", player->position_x, player->position_y);
	gameplay_bombs_add(player, player->position_x, player->position_y);
	player->placed_bombs++;
}

void gameplay_players_use_item(void)
{
	gameplay_players_player_t *player = NULL;
	gameplay_items_item_t item = EMPTY;
	
	player = gameplay_players_get_user();
	if(player == NULL)
	{
		core_error("Failed to find user controlled player.");
		return;
	}
	
	item = gameplay_get_item(player->position_x, player->position_y);
	if(item != EMPTY)
	{
		player->item = item;
	}
	
	switch(player->item)
	{
		case EMPTY:
		{
			break;
		}
		case HEALTH:
		{
			core_debug("Using health power up.");
			player->health_points++;
			player->item = EMPTY;
			break;
		}
		case EXTRA_BOMB:
		{
			core_debug("Using extra bomb power up.");
			player->placeable_bombs++;
			player->item = EMPTY;
			break;
		}
		case SPEED:
		{
			core_debug("Using speed power up.");
			player->movement_cooldown = 0;
			player->movement_cooldown_initial = GAMEPLAY_PLAYERS_MOVEMENT_COOLDOWN_POWERED;
			player->item = EMPTY;
			break;
		}
		case SHIELD:
		{
			core_debug("Using resistance power up.");
			player->damage_cooldown_initial = GAMEPLAY_PLAYERS_DAMAGE_COOLDOWN_POWERED;
			player->item = EMPTY;
			break;
		}
	}
}

void gameplay_players_harm(int position_x, int position_y)
{
	gameplay_players_player_t *current = NULL;
	
	for(current = gameplay_players_players; current != NULL; current = current->next)
	{
		if(current->position_x == position_x && current->position_y == position_y && current->damage_cooldown == 0 && current->health_points > 0)
		{
			current->health_points--;
			current->damage_cooldown = current->damage_cooldown_initial;
		}
	}
}

void gameplay_players_ai_update(void)
{
	gameplay_players_player_t *current = NULL;
	
	for(current = gameplay_players_players; current != NULL; current = current->next)
	{
		ai_core_update(current);
	}
}
