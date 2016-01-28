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

#include <stdio.h>
#include <stdlib.h>

#include "ai-simulation.h"
#include "ai-pathfinding.h"
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

void ai_simulation_reset_simulated(void)
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
			GAMEPLAY_FIELD(field, x, y).ai_simulation_walkable_simulated = 1;
		}
	}
}

void ai_simulation_explosion_set_unwalkable(int position_x, int position_y, char simulated)
{
	gameplay_field_t *field = NULL;
	
	field = gameplay_get_field();
	
	if(field == NULL)
	{
		return;
	}
	
	if(simulated == 0) // bombs on the field
	{
		GAMEPLAY_FIELD(field, position_x, position_y).ai_simulation_walkable = 0;
	}
	else // virtual bombs
	{
		GAMEPLAY_FIELD(field, position_x, position_y).ai_simulation_walkable_simulated = 0;
	}
}

// simulates an explosion. after that function it is possible to test where the
// explosion will be. this can be superpositioned (called multiple times).
void ai_simulation_explosion(int position_x, int position_y, int explosion_radius, char simulated)
{
	int x = 0;
	int y = 0;
	
	// if(simulated == 0)
	// {
	// 	core_debug("Simulate at (%i, %i)", position_x, position_y);
	// }
	
	for(x = position_x; x < GAMEPLAY_FIELD_WIDTH && x < position_x + explosion_radius && gameplay_get_walkable(x, position_y, 1); x++)
	{
		ai_simulation_explosion_set_unwalkable(x, position_y, simulated);
	}
	
	for(x = position_x - 1; x > 0 && x > position_x - explosion_radius && gameplay_get_walkable(x, position_y, 1); x--)
	{
		ai_simulation_explosion_set_unwalkable(x, position_y, simulated);
	}
	
	for(y = position_y; y < GAMEPLAY_FIELD_HEIGHT && y < position_y + explosion_radius && gameplay_get_walkable(position_x, y, 1); y++)
	{
		ai_simulation_explosion_set_unwalkable(position_x, y, simulated);
	}
	
	for(y = position_y - 1; y > 0 && y > position_y - explosion_radius && gameplay_get_walkable(position_x, y, 1); y--)
	{
		ai_simulation_explosion_set_unwalkable(position_x, y, simulated);
	}
}

void ai_simulation_copy_fire(void)
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
			if(GAMEPLAY_FIELD(field, x, y).fire == 1)
			{
				ai_simulation_explosion_set_unwalkable(x, y, 0);
			}
		}
	}
}

// validates if a tile is valid. on a valid tile may be placed a bomb. the
// validation tests if there are spots to hide from the explosion
int ai_simulation_validate_tile(int explosion_radius, int position_x, int position_y)
{
	int x = 0;
	int y = 0;
	int count_hiding_places = 0;
	gameplay_field_t *field = NULL;
	
	field = gameplay_get_field();
	
	if(field == NULL)
	{
		return 0;
	}
	
	ai_simulation_reset_simulated();
	ai_simulation_explosion(position_x, position_y, explosion_radius, 1);
	
	for(y = 0; y < GAMEPLAY_FIELD_HEIGHT; y++)
	{
		for(x = 0; x < GAMEPLAY_FIELD_WIDTH; x++)
		{
			if(gameplay_get_walkable(x, y, 0) == 1 && GAMEPLAY_FIELD(field, x, y).ai_simulation_walkable == 1 && GAMEPLAY_FIELD(field, x, y).ai_simulation_walkable_simulated == 1 && ai_pathfinding_move_to_length(position_x, position_y, x, y, 1) != -1)
			{
				count_hiding_places++;
			}
		}
	}
	
	// core_debug("Found %i hiding places for (%i, %i)", count_hiding_places, position_x, position_y);
	
	return count_hiding_places;
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

int ai_simulation_get_walkable(int position_x, int position_y)
{
	gameplay_field_t *field = NULL;
	
	field = gameplay_get_field();
	
	if(field == NULL)
	{
		return 0;
	}
	
	return GAMEPLAY_FIELD(field, position_x, position_y).ai_simulation_walkable;
}
