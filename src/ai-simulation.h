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

#ifndef __AI_SIMULATION_H__
#define __AI_SIMULATION_H__

void ai_simulation_reset(void);
void ai_simulation_reset_simulated(void);
void ai_simulation_explosion(int position_x, int position_y, int explosion_radius, char simulated);
void ai_simulation_copy_fire(void);
int ai_simulation_validate_tile(int explosion_radius, int position_x, int position_y);
void ai_simulation_print(void);
int ai_simulation_get_walkable(int position_x, int position_y);

#endif /* __AI_SIMULATION_H__ */
