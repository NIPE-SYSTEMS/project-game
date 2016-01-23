#include <stdlib.h>

#include "ai-core.h"
#include "ai-pathfinding.h"
#include "ai-simulation.h"
#include "gameplay-players.h"
#include "gameplay.h"
#include "core.h"

// static char ai_core_ai_enabled = 0;

void ai_core_update(gameplay_players_player_t *player)
{
	int x = 0;
	int y = 0;
	ai_jobs_t *job = NULL;
	
	if(player->type != GAMEPLAY_PLAYERS_TYPE_AI)
	{
		return;
	}
	
	if(player->jobs != NULL)
	{
		ai_jobs_free(&(player->jobs));
	}
	
	// all tiles are potential bomb drop spots
	for(y = 0; y < GAMEPLAY_FIELD_HEIGHT; y++)
	{
		for(x = 0; x < GAMEPLAY_FIELD_WIDTH; x++)
		{
			job = ai_jobs_allocate(x, y, BOMB_DROP);
			ai_jobs_insert(&(player->jobs), job);
		}
	}
	
	// remove tiles which are not walkable (a.k.a. floor tiles)
	for(y = 0; y < GAMEPLAY_FIELD_HEIGHT; y++)
	{
		for(x = 0; x < GAMEPLAY_FIELD_WIDTH; x++)
		{
			if(gameplay_get_walkable(x, y) == 0)
			{
				// core_debug("Remove (%i, %i), cause: walkable", x, y);
				ai_jobs_remove(&(player->jobs), x, y, BOMB_DROP);
			}
		}
	}
	
	// remove tiles which are not accessable
	for(y = 0; y < GAMEPLAY_FIELD_HEIGHT; y++)
	{
		for(x = 0; x < GAMEPLAY_FIELD_WIDTH; x++)
		{
			if(ai_pathfinding_move_to_length(player->position_x, player->position_y, x, y) == -1)
			{
				// core_debug("Remove (%i, %i), cause: pathfinding", x, y);
				ai_jobs_remove(&(player->jobs), x, y, BOMB_DROP);
			}
		}
	}
	
	// remove tiles which have no save place for hiding
	for(y = 0; y < GAMEPLAY_FIELD_HEIGHT; y++)
	{
		for(x = 0; x < GAMEPLAY_FIELD_WIDTH; x++)
		{
			if(ai_simulation_validate_tile(player->position_x, player->position_y, player->explosion_radius, x, y) == 0)
			{
				// core_debug("Remove (%i, %i), cause: unsafe", x, y);
				ai_jobs_remove(&(player->jobs), x, y, BOMB_DROP);
			}
		}
	}
	
	// remove current tile
	ai_jobs_remove(&(player->jobs), player->position_x, player->position_y, BOMB_DROP);
	
	// debug output
	ai_jobs_print(player->jobs);
	
	if(player->jobs != NULL && player->movement_cooldown == 0)
	{
		if(ai_pathfinding_move_to_next(player->position_x, player->position_y, player->jobs->position_x, player->jobs->position_y, &x, &y) != -1)
		{
			core_debug("Go to (%i, %i)", x, y);
			player->position_x = x;
			player->position_y = y;
			player->movement_cooldown = player->movement_cooldown_initial;
		}
	}
	
	// core_debug("Pathfinding: Length: %i", ai_pathfinding_move_to_length(1, 1, 1, 5));
}

void ai_core_cleanup(gameplay_players_player_t *player)
{
	if(player->type != GAMEPLAY_PLAYERS_TYPE_AI)
	{
		return;
	}
	
	if(player->jobs != NULL)
	{
		ai_jobs_free(&(player->jobs));
	}
}
