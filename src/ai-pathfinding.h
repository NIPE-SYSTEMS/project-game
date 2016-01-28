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

#ifndef __AI_PATHFINDING_H__
#define __AI_PATHFINDING_H__

#include "gameplay.h"

void ai_pathfinding_reset(void);
void ai_pathfinding_expand_numbers(int x, int y, int number, int ignore_simulated);
int ai_pathfinding_fill_numbers(int start_x, int start_y, int end_x, int end_y, int ignore_simulated);
int ai_pathfinding_link_tile(int x, int y, int number);
int ai_pathfinding_move_to(int start_x, int start_y, int end_x, int end_y, int ignore_simulated);
int ai_pathfinding_move_to_length(int start_x, int start_y, int end_x, int end_y, int ignore_simulated);
int ai_pathfinding_move_to_next(int start_x, int start_y, int end_x, int end_y, int *next_x, int *next_y, int ignore_simulated);

#endif /* __AI_PATHFINDING_H__ */
