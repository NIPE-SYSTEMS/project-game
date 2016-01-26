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

#define GRAPHICS_START_SCREEN_FRAMES 60

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

void graphics_render_debug(void);
int graphics_startscreen(void);
void graphics_game_over_function(void);
void graphics_render_players(void);
void graphics_render_information(void);
void graphics_render_field(void);

#endif /* __GRAPHICS_H__ */
