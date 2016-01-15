#include <stdio.h>
#include <stdlib.h>

#include "ai_jobs.h"

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
		fprintf(stderr, "Failed to allocate job.\n");
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
	
	job_iterator = *root;
	
	// loop until position with equal priority is found
	while(job_iterator->next != NULL)
	{
		if(job_iterator->next == NULL || job_iterator->next->type >= insertion->type)
		{
			break;
		}
		
		job_iterator = job_iterator->next;
	}
	
	// job_iterator is now the position
	job_iterator_next = job_iterator->next;
	job_iterator->next = insertion;
	insertion->next = job_iterator_next;
}

void ai_jobs_print(ai_jobs_t *root)
{
	ai_jobs_t *job_iterator = root;
	
	while(job_iterator != NULL)
	{
		printf("%i, %i, %i\n", job_iterator->type, job_iterator->position_x, job_iterator->position_y);
		
		job_iterator = job_iterator->next;
	}
}

void ai_jobs_free(ai_jobs_t *root)
{
	ai_jobs_t *job_iterator = root;
	ai_jobs_t *job_iterator_backup = NULL;
	
	while(job_iterator != NULL)
	{
		job_iterator_backup = job_iterator->next;
		free(job_iterator);
		job_iterator = job_iterator_backup;
	}
}
