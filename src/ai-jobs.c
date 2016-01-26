#include <stdio.h>
#include <stdlib.h>

#include "ai-jobs.h"
#include "ai-pathfinding.h"
#include "ai-simulation.h"
#include "core.h"

static int ai_jobs_test_occurrence(ai_jobs_t *list, int position_x, int position_y, ai_jobs_type_t type);

static int ai_jobs_test_occurrence(ai_jobs_t *list, int position_x, int position_y, ai_jobs_type_t type)
{
	ai_jobs_t *job_iterator = list;
	
	while(job_iterator != NULL)
	{
		if(job_iterator->type == type && job_iterator->position_x == position_x && job_iterator->position_y == position_y)
		{
			return 1;
		}
		
		job_iterator = job_iterator->next;
	}
	
	return 0;
}

ai_jobs_t *ai_jobs_allocate(int position_x, int position_y, ai_jobs_type_t type)
{
	ai_jobs_t *job = NULL;
	
	// allocate new job
	job = malloc(sizeof(ai_jobs_t));
	if(job == NULL)
	{
		core_error("Failed to allocate job.");
		return NULL;
	}
	
	// copy data
	job->position_x = position_x;
	job->position_y = position_y;
	job->type = type;
	job->score = 0;
	job->next = NULL;
	
	return job;
}

void ai_jobs_insert(ai_jobs_t **root, ai_jobs_t *insertion)
{
	ai_jobs_t *job_iterator = NULL;
	ai_jobs_t *job_iterator_next = NULL;
	
	// error handling
	if(insertion == NULL)
	{
		return;
	}
	
	// prepend to start if the list is empty
	if(*root == NULL)
	{
		*root = insertion;
		return;
	}
	
	// inhibit redundant insertion
	if(ai_jobs_test_occurrence(*root, insertion->position_x, insertion->position_y, insertion->type) == 1)
	{
		return;
	}
	
	// prepend to start if it has the highest priority
	if((*root)->type == insertion->type)
	{
		insertion->next = *root;
		*root = insertion;
		return;
	}
	
	// loop until position with equal priority is found
	for(job_iterator = *root; job_iterator->next != NULL; job_iterator = job_iterator->next)
	{
		if(job_iterator->next == NULL || job_iterator->next->type >= insertion->type)
		{
			break;
		}
	}
	
	// job_iterator is now the position
	job_iterator_next = job_iterator->next;
	job_iterator->next = insertion;
	insertion->next = job_iterator_next;
}

void ai_jobs_print(ai_jobs_t *root)
{
	ai_jobs_t *job_iterator = NULL;
#ifdef DEBUG
	char *serialized_type[] = { "", "ESCAPE", "BOMB_DROP", "POWER_UP" };
#endif /* DEBUG */
	
	core_debug("Jobs:");
	
	for(job_iterator = root; job_iterator != NULL; job_iterator = job_iterator->next)
	{
		core_debug("    (%s, %i, %i, %7.2f)", serialized_type[job_iterator->type], job_iterator->position_x, job_iterator->position_y, job_iterator->score);
	}
	
	// core_debug("End of Jobs");
}

void ai_jobs_free(ai_jobs_t **root)
{
	ai_jobs_t *job_iterator = *root;
	ai_jobs_t *job_iterator_backup = NULL;
	
	while(job_iterator != NULL)
	{
		job_iterator_backup = job_iterator->next;
		free(job_iterator);
		job_iterator = job_iterator_backup;
	}
	
	*root = NULL;
}

void ai_jobs_remove(ai_jobs_t **root, int position_x, int position_y, ai_jobs_type_t type)
{
	ai_jobs_t *job_iterator = NULL;
	ai_jobs_t *next_backup = NULL;
	
	if(root == NULL || *root == NULL)
	{
		return;
	}
	
	if((*root)->position_x == position_x && (*root)->position_y == position_y && (*root)->type == type)
	{
		next_backup = (*root)->next;
		free(*root);
		*root = next_backup;
		return;
	}
	
	for(job_iterator = *root; job_iterator->next != NULL; job_iterator = job_iterator->next)
	{
		if(job_iterator->next != NULL && job_iterator->next->position_x == position_x && job_iterator->next->position_y == position_y && job_iterator->next->type == type)
		{
			next_backup = job_iterator->next->next;
			free(job_iterator->next);
			job_iterator->next = next_backup;
			return;
		}
	}
}

ai_jobs_t *ai_jobs_get_optimal(ai_jobs_t *root, int position_x_user, int position_y_user, int position_x_ai, int position_y_ai)
{
	ai_jobs_t *job_iterator = NULL;
	int distance_to_player = 0;
	int distance_to_walk = 0;
	float saved_score = -1;
	ai_jobs_t *job_optimal = NULL;
	
	for(job_iterator = root; job_iterator != NULL; job_iterator = job_iterator->next)
	{
		job_iterator->score = 0;
		
		switch(job_iterator->type)
		{
			case ESCAPE:
			{
				distance_to_walk = ai_pathfinding_move_to_length(position_x_ai, position_y_ai, job_iterator->position_x, job_iterator->position_y, 2);
				distance_to_player = ai_pathfinding_move_to_length(job_iterator->position_x, job_iterator->position_y, position_x_user, position_y_user, 2);
				
				if(distance_to_walk == -1)
				{
					job_iterator->score += 25;
				}
				else
				{
					job_iterator->score += distance_to_walk * 0.1;
				}
				
				if(distance_to_player != -1)
				{
					job_iterator->score += distance_to_player * 0.05;
				}
				
				if(ai_simulation_get_walkable(job_iterator->position_x, job_iterator->position_y) == 0)
				{
					job_iterator->score += 5;
				}
				
				break;
			}
			case BOMB_DROP:
			{
				distance_to_walk = ai_pathfinding_move_to_length(position_x_ai, position_y_ai, job_iterator->position_x, job_iterator->position_y, 0);
				distance_to_player = ai_pathfinding_move_to_length(job_iterator->position_x, job_iterator->position_y, position_x_user, position_y_user, 0);
				
				if(distance_to_player == -1)
				{
					job_iterator->score += 25;
				}
				else
				{
					job_iterator->score += distance_to_player * 0.2;
				}
				
				job_iterator->score += distance_to_walk * 0.1;
				
				break;
			}
		}
		
		if(job_iterator->score > saved_score)
		{
			saved_score = job_iterator->score;
			job_optimal = job_iterator;
		}
	}
	
	for(job_iterator = root; job_iterator != NULL; job_iterator = job_iterator->next)
	{
		if(job_iterator->score < saved_score)
		{
			saved_score = job_iterator->score;
			job_optimal = job_iterator;
		}
	}
	
	return job_optimal;
}
