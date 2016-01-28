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

#include "ai-pathfinding.h"
#include "gameplay.h"
#include "core.h"

/**
 * This function resets all pathfinding properties of all tiles in the field.
 */
void ai_pathfinding_reset(void)
{
	int x = 0;
	int y = 0;
	gameplay_field_t *field = NULL;
	
	field = gameplay_get_field();
	
	if(field == NULL)
	{
		return;
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
}

/**
 * This function tries to expand numbers to neighbor tiles. It respects if it
 * should expand numbers to blocked tiles from the simulation.
 * 
 * @param x The x coordinate of a tile.
 * @param y The y coordinate of a tile.
 * @param number The number which should be set to obtainable tiles.
 * @param ignore_simulated A setting to set which tiles should be ignored by
 *                         the function. 0 means that all simulated tiles are
 *                         interpreted as unobtainable tiles. 1 means that
 *                         normal simulated tiles are ignored (are obtainable).
 *                         2 means that all simulated tiles are ignored (are
 *                         obtainable).
 */
void ai_pathfinding_expand_numbers(int x, int y, int number, int ignore_simulated)
{
	gameplay_field_t *field = NULL;
	
	field = gameplay_get_field();
	
	if(field == NULL)
	{
		return;
	}
	
	// try north
	if(y > 0 && GAMEPLAY_FIELD(field, x, y - 1).ai_pathfinding_number == -1 && gameplay_get_walkable(x, y - 1, 0) == 1 && (ignore_simulated > 1 || (ignore_simulated < 2 && GAMEPLAY_FIELD(field, x, y - 1).ai_simulation_walkable == 1)) && (ignore_simulated > 0 || (ignore_simulated == 0 && GAMEPLAY_FIELD(field, x, y - 1).ai_simulation_walkable_simulated == 1)))
	{
		GAMEPLAY_FIELD(field, x, y - 1).ai_pathfinding_number = number;
	}
	
	// try east
	if(x < GAMEPLAY_FIELD_WIDTH - 1 && GAMEPLAY_FIELD(field, x + 1, y).ai_pathfinding_number == -1 && gameplay_get_walkable(x + 1, y, 0) == 1 && (ignore_simulated > 1 || (ignore_simulated < 2 && GAMEPLAY_FIELD(field, x + 1, y).ai_simulation_walkable == 1)) && (ignore_simulated > 0 || (ignore_simulated == 0 && GAMEPLAY_FIELD(field, x + 1, y).ai_simulation_walkable_simulated == 1)))
	{
		GAMEPLAY_FIELD(field, x + 1, y).ai_pathfinding_number = number;
	}
	
	// try south
	if(y < GAMEPLAY_FIELD_HEIGHT - 1 && GAMEPLAY_FIELD(field, x, y + 1).ai_pathfinding_number == -1 && gameplay_get_walkable(x, y + 1, 0) == 1 && (ignore_simulated > 1 || (ignore_simulated < 2 && GAMEPLAY_FIELD(field, x, y + 1).ai_simulation_walkable == 1)) && (ignore_simulated > 0 || (ignore_simulated == 0 && GAMEPLAY_FIELD(field, x, y + 1).ai_simulation_walkable_simulated == 1)))
	{
		GAMEPLAY_FIELD(field, x, y + 1).ai_pathfinding_number = number;
	}
	
	// try west
	if(x > 0 && GAMEPLAY_FIELD(field, x - 1, y).ai_pathfinding_number == -1 && gameplay_get_walkable(x - 1, y, 0) == 1 && (ignore_simulated > 1 || (ignore_simulated < 2 && GAMEPLAY_FIELD(field, x - 1, y).ai_simulation_walkable == 1)) && (ignore_simulated > 0 || (ignore_simulated == 0 && GAMEPLAY_FIELD(field, x - 1, y).ai_simulation_walkable_simulated == 1)))
	{
		GAMEPLAY_FIELD(field, x - 1, y).ai_pathfinding_number = number;
	}
}

/**
 * This function fills the field with pathfinding numbers for the wavefront/
 * floodfill algorithm. It fills the numbers from a given start position to
 * a given target/end position.
 * 
 * @param start_x The x coordinate of the start position.
 * @param start_y The y coordinate of the start position.
 * @param end_x The x coordinate of the end position.
 * @param end_y The y coordinate of the end position.
 * @param ignore_simulated A setting to set which tiles should be ignored by
 *                         the function. 0 means that all simulated tiles are
 *                         interpreted as unobtainable tiles. 1 means that
 *                         normal simulated tiles are ignored (are obtainable).
 *                         2 means that all simulated tiles are ignored (are
 *                         obtainable).
 * @return 0 on succes, -1 on error.
 */
int ai_pathfinding_fill_numbers(int start_x, int start_y, int end_x, int end_y, int ignore_simulated)
{
	int number = 0;
	int exit = 0;
	int x = 0;
	int y = 0;
	int count = 0;
	gameplay_field_t *field = NULL;
	
	field = gameplay_get_field();
	
	if(field == NULL)
	{
		return -1;
	}
	
	// core_debug("Pathfinding: Filling numbers (%i, %i) -> (%i, %i)", start_x, start_y, end_x, end_y);
	
	GAMEPLAY_FIELD(field, start_x, start_y).ai_pathfinding_number = 0;
	
	while(exit == 0)
	{
		count = 0;
		
		for(y = 0; exit == 0 && y < GAMEPLAY_FIELD_WIDTH; y++)
		{
			for(x = 0; exit == 0 && x < GAMEPLAY_FIELD_HEIGHT; x++)
			{
				if(GAMEPLAY_FIELD(field, x, y).ai_pathfinding_number == number)
				{
					count++;
					
					if(x == end_x && y == end_y)
					{
						exit = 1;
					}
					else
					{
						ai_pathfinding_expand_numbers(x, y, number + 1, ignore_simulated);
					}
				}
			}
		}
		
		// when no more tiles can be filled with numbers return error
		if(count == 0)
		{
			// core_error("Pathfinding: Impossible to reach target!");
			exit = 1;
			return -1;
		}
		
		number++;
	}
	
	return 0;
}

/**
 * This function links all tiles and produces the shortest way through the
 * field by backtracking the flooded numbers. This function works with
 * recursion.
 * 
 * @param x The x coordinate of the processed tile.
 * @param y The y coordinate of the processed tile.
 * @param number The number which should be searched.
 * @return The length of the calculated subpath.
 */
int ai_pathfinding_link_tile(int x, int y, int number)
{
	gameplay_field_t *field = NULL;
	
	field = gameplay_get_field();
	
	if(field == NULL)
	{
		return -1;
	}
	
	if(number == -1)
	{
		return 0;
	}
	
	// try north
	if(y > 0 && GAMEPLAY_FIELD(field, x, y - 1).ai_pathfinding_number == number)
	{
		GAMEPLAY_FIELD(field, x, y - 1).ai_pathfinding_next = &GAMEPLAY_FIELD(field, x, y);
		return ai_pathfinding_link_tile(x, y - 1, number - 1) + 1;
	}
	// try east
	else if(x < GAMEPLAY_FIELD_WIDTH - 1 && GAMEPLAY_FIELD(field, x + 1, y).ai_pathfinding_number == number)
	{
		GAMEPLAY_FIELD(field, x + 1, y).ai_pathfinding_next = &GAMEPLAY_FIELD(field, x, y);
		return ai_pathfinding_link_tile(x + 1, y, number - 1) + 1;
	}
	// try south
	else if(y < GAMEPLAY_FIELD_HEIGHT - 1 && GAMEPLAY_FIELD(field, x, y + 1).ai_pathfinding_number == number)
	{
		GAMEPLAY_FIELD(field, x, y + 1).ai_pathfinding_next = &GAMEPLAY_FIELD(field, x, y);
		return ai_pathfinding_link_tile(x, y + 1, number - 1) + 1;
	}
	// try west
	else if(x > 0 && GAMEPLAY_FIELD(field, x - 1, y).ai_pathfinding_number == number)
	{
		GAMEPLAY_FIELD(field, x - 1, y).ai_pathfinding_next = &GAMEPLAY_FIELD(field, x, y);
		return ai_pathfinding_link_tile(x - 1, y, number - 1) + 1;
	}
	
	return 0;
}

/**
 * This function floods all numbers and backtracks the shortest way. It returns
 * the length of the shortest way.
 * 
 * @param start_x The x coordinate of the start position.
 * @param start_y The y coordinate of the start position.
 * @param end_x The x coordinate of the end position.
 * @param end_y The y coordinate of the end position.
 * @param ignore_simulated A setting to set which tiles should be ignored by
 *                         the function. 0 means that all simulated tiles are
 *                         interpreted as unobtainable tiles. 1 means that
 *                         normal simulated tiles are ignored (are obtainable).
 *                         2 means that all simulated tiles are ignored (are
 *                         obtainable).
 * @return The length of the calculated path.
 */
int ai_pathfinding_move_to(int start_x, int start_y, int end_x, int end_y, int ignore_simulated)
{
	gameplay_field_t *field = NULL;
	
	field = gameplay_get_field();
	
	if(field == NULL)
	{
		return -1;
	}
	
	if(start_x == end_x && start_y == end_y)
	{
		GAMEPLAY_FIELD(field, start_x, start_y).ai_pathfinding_next = &GAMEPLAY_FIELD(field, start_x, start_y);
		return 0;
	}
	
	ai_pathfinding_reset();
	
	if(ai_pathfinding_fill_numbers(start_x, start_y, end_x, end_y, ignore_simulated) < 0)
	{
		return -1;
	}
	
	return ai_pathfinding_link_tile(end_x, end_y, GAMEPLAY_FIELD(field, end_x, end_y).ai_pathfinding_number - 1);
}

/**
 * This function calculates the length of a path through the field.
 * 
 * @param start_x The x coordinate of the start position.
 * @param start_y The y coordinate of the start position.
 * @param end_x The x coordinate of the end position.
 * @param end_y The y coordinate of the end position.
 * @param ignore_simulated A setting to set which tiles should be ignored by
 *                         the function. 0 means that all simulated tiles are
 *                         interpreted as unobtainable tiles. 1 means that
 *                         normal simulated tiles are ignored (are obtainable).
 *                         2 means that all simulated tiles are ignored (are
 *                         obtainable).
 * @return The length of the calculated path.
 */
int ai_pathfinding_move_to_length(int start_x, int start_y, int end_x, int end_y, int ignore_simulated)
{
	return ai_pathfinding_move_to(start_x, start_y, end_x, end_y, ignore_simulated);
}

/**
 * This function calculates the next element of a path.
 * 
 * @param start_x The x coordinate of the start position.
 * @param start_y The y coordinate of the start position.
 * @param end_x The x coordinate of the end position.
 * @param end_y The y coordinate of the end position.
 * @param next_x The x coordinate of the next position (write by pointer).
 * @param next_y The y coordinate of the next position (write by pointer).
 * @param ignore_simulated A setting to set which tiles should be ignored by
 *                         the function. 0 means that all simulated tiles are
 *                         interpreted as unobtainable tiles. 1 means that
 *                         normal simulated tiles are ignored (are obtainable).
 *                         2 means that all simulated tiles are ignored (are
 *                         obtainable).
 * @return The length of the calculated path.
 */
int ai_pathfinding_move_to_next(int start_x, int start_y, int end_x, int end_y, int *next_x, int *next_y, int ignore_simulated)
{
	int return_length = 0;
	gameplay_field_t *field = NULL;
	
	field = gameplay_get_field();
	
	if(field == NULL)
	{
		return -1;
	}
	
	return_length = ai_pathfinding_move_to(start_x, start_y, end_x, end_y, ignore_simulated);
	if(return_length < 0)
	{
		return -1;
	}
	
	if(GAMEPLAY_FIELD(field, start_x, start_y).ai_pathfinding_next == NULL)
	{
		return -1;
	}
	
	*next_x = GAMEPLAY_FIELD(field, start_x, start_y).ai_pathfinding_next->ai_position_x;
	*next_y = GAMEPLAY_FIELD(field, start_x, start_y).ai_pathfinding_next->ai_position_y;
	
	return return_length;
}
