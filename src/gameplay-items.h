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

#ifndef __GAMEPLAY_ITEMS_H__
#define __GAMEPLAY_ITEMS_H__

#define GAMEPLAY_ITEMS_ITEM_DESPAWN 100

typedef enum gameplay_items_item_type_e
{
	EMPTY = 2,
	HEALTH = 10,
	EXTRA_BOMB = 8,
	FIRE = 9,
	SPEED = 11,
	SHIELD = 13,
} gameplay_items_item_type_t;

typedef struct gameplay_items_item_s
{
	gameplay_items_item_type_t type;
	int position_x;
	int position_y;
	int despawn_timer;
	struct gameplay_items_item_s *next;
} gameplay_items_item_t;

void gameplay_items_add_item(gameplay_items_item_type_t type, int position_x, int position_y);
void gameplay_items_cleanup(void);
void gameplay_items_remove(int position_x, int position_y);
void gameplay_items_item_update();
int gameplay_items_item_placed(int position_x, int position_y);
gameplay_items_item_type_t gameplay_items_get_item_type (int position_x, int position_y);
int gameplay_items_test_remove(int position_x, int position_y);

#endif /* __GAMEPLAY_ITEMS_H__ */
