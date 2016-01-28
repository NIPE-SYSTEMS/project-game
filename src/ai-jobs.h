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
