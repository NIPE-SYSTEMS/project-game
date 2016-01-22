#include <stdio.h>
#include <stdlib.h>

#include "ai-jobs.h"
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
	char *serialized_type[] = { "", "ESCAPE", "BOMB_DROP", "POWER_UP" };
	
	core_debug("Jobs:");
	
	for(job_iterator = root; job_iterator != NULL; job_iterator = job_iterator->next)
	{
		core_debug("    (type: %s, x: %i, y: %i)", serialized_type[job_iterator->type], job_iterator->position_x, job_iterator->position_y);
	}
	
	core_debug("End of Jobs");
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
