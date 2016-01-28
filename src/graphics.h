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

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#define GRAPHICS_OFFSET_X 8
#define GRAPHICS_OFFSET_Y 4
#define GRAPHICS_SPRITE_WIDTH 5
#define GRAPHICS_SPRITE_HEIGHT 3
#define GRAPHICS_HEALTH_X 77
#define GRAPHICS_HEALTH_Y 3
#define GRAPHICS_HEALTH_OFFSET_X 6
#define GRAPHICS_HEALTH_OFFSET_Y 2
#define GRAPHICS_SPINNING_ANIMATION_X 99
#define GRAPHICS_SPINNING_ANIMATION_Y 36
#define GRAPHICS_DEBUG_X 75
#define GRAPHICS_DEBUG_Y 1

#define GRAPHICS_QR_CODE_SIZE 31
#define GRAPHICS_QR_CODE_OFFSET_X 5
#define GRAPHICS_QR_CODE_OFFSET_Y 1
#define GRAPHICS_QR_CODE_PIXEL_SIZE_WIDTH 2
#define GRAPHICS_QR_CODE_PIXEL_SIZE_HEIGHT 1

#define GRAPHICS_MENU_KEYS_DELAY 35
#define GRAPHICS_MENU_KEYS_X 15
#define GRAPHICS_MENU_KEYS_Y 23
#define GRAPHICS_MENU_KEYS_TEXT_X 29
#define GRAPHICS_MENU_KEYS_TEXT_Y 21
#define GRAPHICS_MENU_LOGO_X 7
#define GRAPHICS_MENU_LOGO_Y 2
#define GRAPHICS_MENU_START_X 23
#define GRAPHICS_MENU_START_Y 17

typedef enum graphics_sprites_e
{
	GRAPHICS_SPRITES_UNDESTROYABLE = 0,
	GRAPHICS_SPRITES_DESTROYABLE = 1,
	GRAPHICS_SPRITES_DESTROYED = 2,
	GRAPHICS_SPRITES_PLAYER = 3,
	GRAPHICS_SPRITES_PLAYER_STANDING = 4,
	GRAPHICS_SPRITES_ENEMY = 5,
	GRAPHICS_SPRITES_ENEMY_STANDING = 6,
	GRAPHICS_SPRITES_BOMB = 7,
	GRAPHICS_SPRITES_BOMB_UP = 8,
	GRAPHICS_SPRITES_FIRE = 9,
	GRAPHICS_SPRITES_HEART = 10,
	GRAPHICS_SPRITES_SNEAKER = 11,
	GRAPHICS_SPRITES_UP = 12,
	GRAPHICS_SPRITES_SHIELD = 13,
	GRAPHICS_SPRITES_EXPLOSION_1 = 14,
	GRAPHICS_SPRITES_EXPLOSION_2 = 15,
	GRAPHICS_SPRITES_BOMBS_PLACABLE = 16,
	GRAPHICS_SPRITES_SPEED = 17
} graphics_sprites_t;

typedef struct graphics_sprite_s
{
	graphics_sprites_t type;
	char *path;
	int width;
	int height;
	char *data;
} graphics_sprite_t;

void graphics_render_menu(void);
void graphics_render_debug(void);
int graphics_render_start_screen(void);
void graphics_render_game_over_screen(void);
void graphics_render_players(void);
void graphics_render_information(void);
void graphics_render_field(void);
void graphics_render_qr_code(void);

#endif /* __GRAPHICS_H__ */
