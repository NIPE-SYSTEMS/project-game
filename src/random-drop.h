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

#ifndef __RANDOM_DROP_H__
#define __RANDOM_DROP_H__

#define RANDOM_DROP_ACCURACY 10000 // a lower value will decrease the accuracy

typedef struct random_drop_t
{
	unsigned int id;
	float probability; // [0, 1]
} random_drop_t;

void random_drop_init(void);
random_drop_t *random_drop_choose(random_drop_t *drop_list, size_t drop_list_amount);

#endif /* __RANDOM_DROP_H__ */