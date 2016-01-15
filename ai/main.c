#include <stdio.h>
#include <stdlib.h>

#include "ai_jobs.h"

typedef struct ai_field_s
{
	int position_x;
	int position_y;
	char wall;
	char explosion;
	struct ai_field_s *pathfinding_next;
	int pathfinding_number;
} ai_field_t;

int pos_x = 4;
int pos_y = 4;

ai_field_t field[9][9];

static void ai_pathfinding_reset(void)
{
	int x = 0;
	int y = 0;
	
	for(x = 0; x < 9; x++)
	{
		for(y = 0; y < 9; y++)
		{
			field[x][y].pathfinding_next = NULL;
			field[x][y].pathfinding_number = -1;
		}
	}
}

static void ai_pathfinding_expand_numbers(int x, int y, int number)
{
	// try north
	if(y > 0 && field[x][y - 1].pathfinding_number == -1 && field[x][y - 1].wall == 0)
	{
		field[x][y - 1].pathfinding_number = number;
	}
	
	// try east
	if(x < 8 && field[x + 1][y].pathfinding_number == -1 && field[x + 1][y].wall == 0)
	{
		field[x + 1][y].pathfinding_number = number;
	}
	
	// try south
	if(y < 8 && field[x][y + 1].pathfinding_number == -1 && field[x][y + 1].wall == 0)
	{
		field[x][y + 1].pathfinding_number = number;
	}
	
	// try west
	if(x > 0 && field[x - 1][y].pathfinding_number == -1 && field[x - 1][y].wall == 0)
	{
		field[x - 1][y].pathfinding_number = number;
	}
}

static int ai_pathfinding_fill_numbers(int start_x, int start_y, int end_x, int end_y)
{
	int number = 0;
	int exit = 0;
	int x = 0;
	int y = 0;
	int count = 0;
	
	field[start_x][start_y].pathfinding_number = 0;
	
	while(exit == 0)
	{
		count = 0;
		
		for(x = 0; exit == 0 && x < 9; x++)
		{
			for(y = 0; exit == 0 && y < 9; y++)
			{
				if(field[x][y].pathfinding_number == number)
				{
					count++;
					
					if(x == end_x && y == end_y)
					{
						exit = 1;
					}
					else
					{
						ai_pathfinding_expand_numbers(x, y, number + 1);
					}
				}
			}
		}
		
		// when no more tiles can be filled with numbers return error
		if(count == 0)
		{
			printf("Impossible to reach target!\n");
			exit = 1;
			return -1;
		}
		
		number++;
	}
	
	return 0;
}

static int ai_pathfinding_link_tile(int x, int y, int number)
{
	// try north
	if(y > 0 && field[x][y - 1].pathfinding_number == number)
	{
		field[x][y - 1].pathfinding_next = &field[x][y];
		return ai_pathfinding_link_tile(x, y - 1, number - 1) + 1;
	}
	// try east
	else if(x < 8 && field[x + 1][y].pathfinding_number == number)
	{
		field[x + 1][y].pathfinding_next = &field[x][y];
		return ai_pathfinding_link_tile(x + 1, y, number - 1) + 1;
	}
	// try south
	else if(y < 8 && field[x][y + 1].pathfinding_number == number)
	{
		field[x][y + 1].pathfinding_next = &field[x][y];
		return ai_pathfinding_link_tile(x, y + 1, number - 1) + 1;
	}
	// try west
	else if(x > 0 && field[x - 1][y].pathfinding_number == number)
	{
		field[x - 1][y].pathfinding_next = &field[x][y];
		return ai_pathfinding_link_tile(x - 1, y, number - 1) + 1;
	}
	
	return 0;
}

// static void ai_pathfinding_print(int start_x, int start_y)
// {
// 	ai_field_t *current = &field[start_x][start_y];
	
// 	while(current != NULL)
// 	{
// 		printf("%i, %i\n", current->position_x, current->position_y);
		
// 		current = current->pathfinding_next;
// 	}
// }

static int ai_pathfinding_move_to(int start_x, int start_y, int end_x, int end_y)
{
	ai_pathfinding_reset();
	
	if(ai_pathfinding_fill_numbers(start_x, start_y, end_x, end_y) < 0)
	{
		return -1;
	}
	
	return ai_pathfinding_link_tile(end_x, end_y, field[end_x][end_y].pathfinding_number - 1);
}

static int ai_pathfinding_move_to_length(int start_x, int start_y, int end_x, int end_y)
{
	return ai_pathfinding_move_to(start_x, start_y, end_x, end_y);
}

static int ai_pathfinding_move_to_next(int start_x, int start_y, int end_x, int end_y, int *next_x, int *next_y)
{
	int return_length = ai_pathfinding_move_to(start_x, start_y, end_x, end_y);
	
	if(return_length < 0)
	{
		return -1;
	}
	
	*next_x = field[start_x][start_y].pathfinding_next->position_x;
	*next_y = field[start_x][start_y].pathfinding_next->position_y;
	
	return return_length;
}

int main(void)
{
	// ai_jobs_t *job_list = NULL;
	
	// ai_jobs_t *job = ai_jobs_allocate(0, 0, ESCAPE);
	// ai_jobs_insert(&job_list, job);
	// job = ai_jobs_allocate(1, 0, BOMB_DROP);
	// ai_jobs_insert(&job_list, job);
	// job = ai_jobs_allocate(2, 0, POWER_UP);
	// ai_jobs_insert(&job_list, job);
	// job = ai_jobs_allocate(3, 0, BOMB_DROP);
	// ai_jobs_insert(&job_list, job);
	// job = ai_jobs_allocate(4, 0, POWER_UP);
	// ai_jobs_insert(&job_list, job);
	// job = ai_jobs_allocate(5, 0, ESCAPE);
	// ai_jobs_insert(&job_list, job);
	// job = ai_jobs_allocate(6, 0, BOMB_DROP);
	// ai_jobs_insert(&job_list, job);
	// job = ai_jobs_allocate(4, 0, POWER_UP);
	// ai_jobs_insert(&job_list, job);
	// free(job);
	// ai_jobs_print(job_list);
	// ai_jobs_free(job_list);
	
	int x = 0;
	int y = 0;
	
	for(x = 0; x < 9; x++)
	{
		for(y = 0; y < 9; y++)
		{
			field[x][y].position_x = x;
			field[x][y].position_y = y;
			field[x][y].wall = 0;
			field[x][y].explosion = 0;
			field[x][y].pathfinding_next = NULL;
			field[x][y].pathfinding_number = -1;
		}
	}
	
	field[1][1].wall = 1;
	field[2][1].wall = 1;
	field[3][1].wall = 1;
	// field[1][2].wall = 1;
	field[3][2].wall = 1;
	field[1][3].wall = 1;
	field[2][3].wall = 1;
	field[3][3].wall = 1;
	
	printf("Lenght: %i\n", ai_pathfinding_move_to_length(pos_x, pos_y, 2, 2));
	ai_pathfinding_move_to_next(pos_x, pos_y, 2, 2, &x, &y);
	
	printf("Current tile: (%i, %i), next tile: (%i, %i)\n", pos_x, pos_y, x, y);
	
	for(x = 0; x < 9; x++)
	{
		for(y = 0; y < 9; y++)
		{
			if(field[x][y].pathfinding_next != NULL)
			{
				printf(" \e[1;31m%2i\e[0m ", field[x][y].pathfinding_number);
			}
			else if(field[x][y].wall == 0)
			{
				printf(" \e[1;32m%2i\e[0m ", field[x][y].pathfinding_number);
			}
			else
			{
				printf(" \e[1;34m%2i\e[0m ", field[x][y].pathfinding_number);
			}
		}
		printf("\n");
	}
	
	return 0;
}