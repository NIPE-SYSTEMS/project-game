#ifndef __AI_PATHFINDING_H__
#define __AI_PATHFINDING_H__

#include "gameplay.h"

void ai_pathfinding_reset(void);
void ai_pathfinding_expand_numbers(int x, int y, int number);
int ai_pathfinding_fill_numbers(int start_x, int start_y, int end_x, int end_y);
int ai_pathfinding_link_tile(int x, int y, int number);
int ai_pathfinding_move_to(int start_x, int start_y, int end_x, int end_y);
int ai_pathfinding_move_to_length(int start_x, int start_y, int end_x, int end_y);
int ai_pathfinding_move_to_next(int start_x, int start_y, int end_x, int end_y, int *next_x, int *next_y);

#endif /* __AI_PATHFINDING_H__ */
