#include <stdio.h>
#include <stdlib.h>

#include "ai-pathfinding.h"
#include "gameplay.h"
#include "core.h"

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

void ai_pathfinding_expand_numbers(int x, int y, int number, int ignore_simulated)
{
	gameplay_field_t *field = NULL;
	
	field = gameplay_get_field();
	
	if(field == NULL)
	{
		return;
	}
	
	// core_debug("Pathfinding: Expanding at (%i, %i): %i", x, y, number);
	
	// try north
	if(y > 0 && GAMEPLAY_FIELD(field, x, y - 1).ai_pathfinding_number == -1 && gameplay_get_walkable(x, y - 1) == 1 && (ignore_simulated > 1 || (ignore_simulated < 2 && GAMEPLAY_FIELD(field, x, y - 1).ai_simulation_walkable == 1)) && (ignore_simulated > 0 || (ignore_simulated == 0 && GAMEPLAY_FIELD(field, x, y - 1).ai_simulation_walkable_simulated == 1)))
	{
		GAMEPLAY_FIELD(field, x, y - 1).ai_pathfinding_number = number;
	}
	
	// try east
	if(x < GAMEPLAY_FIELD_WIDTH - 1 && GAMEPLAY_FIELD(field, x + 1, y).ai_pathfinding_number == -1 && gameplay_get_walkable(x + 1, y) == 1 && (ignore_simulated > 1 || (ignore_simulated < 2 && GAMEPLAY_FIELD(field, x + 1, y).ai_simulation_walkable == 1)) && (ignore_simulated > 0 || (ignore_simulated == 0 && GAMEPLAY_FIELD(field, x + 1, y).ai_simulation_walkable_simulated == 1)))
	{
		GAMEPLAY_FIELD(field, x + 1, y).ai_pathfinding_number = number;
	}
	
	// try south
	if(y < GAMEPLAY_FIELD_HEIGHT - 1 && GAMEPLAY_FIELD(field, x, y + 1).ai_pathfinding_number == -1 && gameplay_get_walkable(x, y + 1) == 1 && (ignore_simulated > 1 || (ignore_simulated < 2 && GAMEPLAY_FIELD(field, x, y + 1).ai_simulation_walkable == 1)) && (ignore_simulated > 0 || (ignore_simulated == 0 && GAMEPLAY_FIELD(field, x, y + 1).ai_simulation_walkable_simulated == 1)))
	{
		GAMEPLAY_FIELD(field, x, y + 1).ai_pathfinding_number = number;
	}
	
	// try west
	if(x > 0 && GAMEPLAY_FIELD(field, x - 1, y).ai_pathfinding_number == -1 && gameplay_get_walkable(x - 1, y) == 1 && (ignore_simulated > 1 || (ignore_simulated < 2 && GAMEPLAY_FIELD(field, x - 1, y).ai_simulation_walkable == 1)) && (ignore_simulated > 0 || (ignore_simulated == 0 && GAMEPLAY_FIELD(field, x - 1, y).ai_simulation_walkable_simulated == 1)))
	{
		GAMEPLAY_FIELD(field, x - 1, y).ai_pathfinding_number = number;
	}
}

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

int ai_pathfinding_link_tile(int x, int y, int number)
{
	gameplay_field_t *field = NULL;
	
	field = gameplay_get_field();
	
	if(field == NULL)
	{
		return -1;
	}
	
	// core_debug("Pathfinding: Number %i", number);
	
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

// void ai_pathfinding_print(int start_x, int start_y)
// {
// 	ai_field_t *current = &field[start_x][start_y];
	
// 	while(current != NULL)
// 	{
// 		printf("%i, %i\n", current->ai_position_x, current->ai_position_y);
		
// 		current = current->ai_pathfinding_next;
// 	}
// }

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

int ai_pathfinding_move_to_length(int start_x, int start_y, int end_x, int end_y, int ignore_simulated)
{
	return ai_pathfinding_move_to(start_x, start_y, end_x, end_y, ignore_simulated);
}

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
