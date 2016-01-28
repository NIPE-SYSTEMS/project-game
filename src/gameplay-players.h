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

#ifndef __GAMEPLAY_PLAYERS_H__
#define __GAMEPLAY_PLAYERS_H__

#define GAMEPLAY_PLAYERS_MOVEMENT_COOLDOWN 5
//#define GAMEPLAY_PLAYERS_MOVEMENT_COOLDOWN_POWERED 3
#define GAMEPLAY_PLAYERS_DAMAGE_COOLDOWN 10
#define GAMEPLAY_PLAYERS_DAMAGE_COOLDOWN_POWERED 50
#define GAMEPLAY_PLAYERS_DAMAGE_COOLDOWN_START_USER 50
#define GAMEPLAY_PLAYERS_DAMAGE_COOLDOWN_START_AI 0
#define GAMEPLAY_PLAYERS_EXPLOSION_RADIUS 2
#define GAMEPLAY_PLAYERS_AMOUNT_BOMBS 1
#define GAMEPLAY_PLAYERS_AI_START_COOLDOWN 100

#include "gameplay-items.h"
#include "ai-jobs.h"

typedef enum gameplay_players_type_e
{
	GAMEPLAY_PLAYERS_TYPE_USER,
	GAMEPLAY_PLAYERS_TYPE_AI,
} gameplay_players_type_t;

typedef struct gameplay_players_player_s
{
	int health_points;
	int movement_cooldown;
	int movement_cooldown_initial;
	int position_y;
	int position_x;
	int placeable_bombs;
	int placed_bombs;
	int explosion_radius;
	gameplay_items_item_type_t item;
	int item_usage_time;
	int damage_cooldown;
	int damage_cooldown_initial;
	gameplay_players_type_t type;
	ai_jobs_t *jobs;
	char turbo_mode_activated;
	struct gameplay_players_player_s *next;
} gameplay_players_player_t;

typedef enum gameplay_players_direction_e
{
	GAMEPLAY_PLAYERS_DIRECTION_UP,
	GAMEPLAY_PLAYERS_DIRECTION_RIGHT,
	GAMEPLAY_PLAYERS_DIRECTION_DOWN,
	GAMEPLAY_PLAYERS_DIRECTION_LEFT
} gameplay_players_direction_t;

void gameplay_players_add(int position_x, int position_y, gameplay_players_type_t type);
void gameplay_players_cleanup(void);
void gameplay_players_update(void);
gameplay_players_player_t *gameplay_players_get_user(void);
void gameplay_players_move(gameplay_players_direction_t direction);
int gameplay_players_amount(void);
int gameplay_players_ai_amount(void);
gameplay_players_player_t *gameplay_players_get(int index);
int gameplay_player_get_player(int position_x, int position_y);
void gameplay_players_place_bomb(gameplay_players_player_t *player);
void gameplay_players_use_item(void);
void gameplay_players_harm(int position_x, int position_y);
void gameplay_players_ai_update(void);
void gameplay_player_activate_turbo_mode(void);

#endif /* __GAMEPLAY_PLAYERS_H__ */
