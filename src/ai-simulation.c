#include <stdio.h>
#include <stdlib.h>

#include "ai-simulation.h"
#include "gameplay.h"
#include "core.h"

void ai_simulation_reset(void)
{
	int x = 0;
	int y = 0;
	gameplay_field_t *field = NULL;
	
	field = gameplay_get_field();
	
	if(field == NULL)
	{
		return;
	}
	
	for(y = 0; y < GAMEPLAY_FIELD_HEIGHT; y++)
	{
		for(x = 0; x < GAMEPLAY_FIELD_WIDTH; x++)
		{
			GAMEPLAY_FIELD(field, x, y).ai_simulation_walkable = 1;
		}
	}
}

// simulates an explosion. after that function it is possible to test where the
// explosion will be. this can be superpositioned (called multiple times).
void ai_simulation_explosion(int position_x, int position_y)
{
	int x = 0;
	int y = 0;
	gameplay_field_t *field = NULL;
	
	field = gameplay_get_field();
	
	if(field == NULL)
	{
		return;
	}
	
	for(x = position_x; x < GAMEPLAY_FIELD_WIDTH && gameplay_get_walkable(x, position_y); x++)
	{
		GAMEPLAY_FIELD(field, x, position_y).ai_simulation_walkable = 0;
	}
	
	for(x = position_x - 1; x > 0 && gameplay_get_walkable(x, position_y); x--)
	{
		GAMEPLAY_FIELD(field, x, position_y).ai_simulation_walkable = 0;
	}
	
	for(y = position_y; y < GAMEPLAY_FIELD_HEIGHT && gameplay_get_walkable(position_x, y); y++)
	{
		GAMEPLAY_FIELD(field, position_x, y).ai_simulation_walkable = 0;
	}
	
	for(y = position_y - 1; y > 0 && gameplay_get_walkable(position_x, y); y--)
	{
		GAMEPLAY_FIELD(field, position_x, y).ai_simulation_walkable = 0;
	}
}

// validates if a tile is valid. on a valid tile may be placed a bomb. the
// validation tests if there are spots to hide from the explosion
int ai_simulation_validate_tile(int position_x, int position_y)
{
	int x = 0;
	int y = 0;
	gameplay_field_t *field = NULL;
	
	field = gameplay_get_field();
	
	if(field == NULL)
	{
		return 0;
	}
	
	// core_debug("Pathfinding: Resetting field");
	
	for(y = 0; y < GAMEPLAY_FIELD_HEIGHT; y++)
	{
		for(x = 0; x < GAMEPLAY_FIELD_WIDTH; x++)
		{
			GAMEPLAY_FIELD(field, x, y).ai_pathfinding_next = NULL;
			GAMEPLAY_FIELD(field, x, y).ai_pathfinding_number = -1;
		}
	}
	
	return 0;
}

void ai_simulation_print(void)
{
	int x = 0;
	int y = 0;
	gameplay_field_t *field = NULL;
	
	field = gameplay_get_field();
	
	if(field == NULL)
	{
		return;
	}
	
	core_debug("Walkables:");
	
	for(y = 0; y < GAMEPLAY_FIELD_HEIGHT; y++)
	{
		for(x = 0; x < GAMEPLAY_FIELD_WIDTH; x++)
		{
			core_debug("    (%i, %i): %i", x, y, GAMEPLAY_FIELD(field, x, y).ai_simulation_walkable);
		}
	}
	
	core_debug("End of Walkables");
}
