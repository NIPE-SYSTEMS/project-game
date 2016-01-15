#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#define GRAPHICS_OFFSET_X 8
#define GRAPHICS_OFFSET_Y 4
#define GRAPHICS_SPRITE_WIDTH 5
#define GRAPHICS_SPRITE_HEIGHT 3
#define GRAPHICS_HEALTH_X 77
#define GRAPHICS_HEALTH_Y 10
#define GRAPHICS_HEALTH_OFFSET_X 6
#define GRAPHICS_HEALTH_OFFSET_Y 2
#define GRAPHICS_SPINNING_ANIMATION_X 99
#define GRAPHICS_SPINNING_ANIMATION_Y 36

typedef enum graphics_fields_sprites_e
{
	UNDESTROYABLE_SPRITE,
	DESTROYABLE_SPRITE,
	DESTROYED_SPRITE,
	PLAYER_SPRITE,
	ENEMY_SPRITE,
	BOMB_SPRITE,
	BOMB_UP_SPRITE,
	FIRE_SPRITE,
	HEART_SPRITE,
	SNEAKER_SPRITE,
	UP_SPRITE,
	SHIELD_SPRITE,
	EXPLOSION_SPRITE,
	EXPLOSION_SPRITE_2,
	PLAYER_STANDING_SPRITE
} graphics_fields_sprites_t;

void graphics_render_sprite(int pos_x, int pos_y, graphics_fields_sprites_t field, char transparency);
void graphics_main(void);

#endif /* __GRAPHICS_H__ */
