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

#ifndef __CORE_H__
#define __CORE_H__

#include <stdarg.h>
#include <stdio.h>

// #define CORE_FRAME_TIME 100000 // -> 10 fps, specified in microseconds (usleep)
#define CORE_FRAME_TIME 75000 // -> 10 fps, specified in microseconds (usleep)

typedef enum core_state_e
{
	CORE_START_SCREEN,
	CORE_MENU,
	CORE_RUNNING,
	CORE_PAUSED,
	CORE_QR_CODE,
	CORE_WIN,
	CORE_GAME_OVER,
	CORE_SHUTDOWN
} core_state_t;

void core_init(void);
void core_main(void);
void core_cleanup(void);

#ifdef DEBUG
void core_debug(char *fmt, ...);
void core_error(char *fmt, ...);
#else
#define core_debug(...) (void)0
#define core_error(...) (void)0
#endif /* DEBUG */

#endif /* __CORE_H__ */