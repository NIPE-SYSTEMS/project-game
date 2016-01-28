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

#ifndef __GAMEPLAY_H__
#define __GAMEPLAY_H__

#define GAMEPLAY_FIELD_HEIGHT 9
#define GAMEPLAY_FIELD_WIDTH 9
#define GAMEPLAY_FIRE_DESPAWN 10

#define GAMEPLAY_FIELD(field, x, y) ((field)[(y) * GAMEPLAY_FIELD_WIDTH + (x)])

#include "gameplay-items.h"
#include "gameplay-players.h"

typedef enum gameplay_field_type_e
{
	WALL = 0,
	DESTRUCTIVE = 1,
	FLOOR = 2,
} gameplay_field_type_t;

typedef struct gameplay_field_s
{
	gameplay_field_type_t type;
	gameplay_items_item_t item;
	int ai_position_x;
	int ai_position_y;
	int ai_pathfinding_number;
	struct gameplay_field_s *ai_pathfinding_next;
	char ai_simulation_walkable;
	char ai_simulation_walkable_simulated;
	int fire;
	int fire_despawn_timer;
} gameplay_field_t;

typedef struct gameplay_turbo_s
{
	int t;
	int u;
	int r;
	int b;
	int o;
} gameplay_turbo_t;

void gameplay_init(void);
void gameplay_cleanup(void);
int gameplay_get_walkable(int position_x, int position_y, char bomb_is_walkable);
//gameplay_items_item_t gameplay_get_item(int position_x, int position_y);
void gameplay_destroy(int position_x, int position_y);
void gameplay_key(char gameplay_pressed_key);
void gameplay_update(void);
void gameplay_explosion(int x, int y);
gameplay_field_t *gameplay_get_field(void);
void gameplay_set_fire(int position_x, int position_y);
int gameplay_get_fire(int position_x, int position_y);

#endif /* __GAMEPLAY_H__ */
