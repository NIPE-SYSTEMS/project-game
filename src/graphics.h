#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

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

int graphics_create_field(int pos_x, int pos_y, int field);
int graphics_main(void);

#endif /* __GRAPHICS_H__ */