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

#ifndef __GAMEPLAY_BOMBS_H__
#define __GAMEPLAY_BOMBS_H__

#include "gameplay.h"
#include "gameplay-players.h"

#define GAMEPLAY_BOMBS_EXPLOSION_TIMEOUT 20
#define GAMEPLAY_BOMBS_FIRE_TIMEOUT 10

typedef struct gameplay_bombs_bomb_s
{
	int position_x;
	int position_y;
	int explosion_timeout;
	//int fire_timeout;
	gameplay_players_player_t *owner;
	struct gameplay_bombs_bomb_s *next;
} gameplay_bombs_bomb_t;

void gameplay_bombs_add(gameplay_players_player_t *player, int position_x, int position_y);
void gameplay_bombs_cleanup(void);
void gameplay_bombs_remove(int position_x, int position_y);
void gameplay_bombs_update(void);
int gameplay_bombs_amount(void);
gameplay_bombs_bomb_t *gameplay_bombs_get(int index);
int gameplay_bombs_get_bomb_placed(int position_x, int position_y);
gameplay_bombs_bomb_t *gameplay_bombs_get_bomb(int position_x, int position_y);
// int gameplay_bombs_get_fire(int position_x, int position_y);
void gameplay_bombs_trigger_explosion(int position_x, int position_y);

#endif /* __GAMEPLAY_BOMBS_H__ */
