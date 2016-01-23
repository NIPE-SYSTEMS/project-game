#ifndef __AI_JOBS_H__
#define __AI_JOBS_H__

typedef enum ai_jobs_type_e
{
	ESCAPE = 1,
	BOMB_DROP = 2
	// POWER_UP = 3
} ai_jobs_type_t;

typedef struct ai_jobs_s
{
	int position_x;
	int position_y;
	ai_jobs_type_t type;
	float score;
	struct ai_jobs_s *next;
} ai_jobs_t;

ai_jobs_t *ai_jobs_allocate(int position_x, int position_y, ai_jobs_type_t type);
void ai_jobs_insert(ai_jobs_t **root, ai_jobs_t *insertion);
void ai_jobs_print(ai_jobs_t *root);
void ai_jobs_free(ai_jobs_t **root);
void ai_jobs_remove(ai_jobs_t **root, int position_x, int position_y, ai_jobs_type_t type);
ai_jobs_t *ai_jobs_get_optimal(ai_jobs_t *root, int position_x_user, int position_y_user, int position_x_ai, int position_y_ai);

#endif /* __AI_JOBS_H__ */
