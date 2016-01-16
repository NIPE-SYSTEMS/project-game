#ifndef __GAMEPLAY_PLAYERS_H__
#define __GAMEPLAY_PLAYERS_H__

#include "gameplay.h"

#define GAMEPLAY_PLAYERS_MOVEMENT_COOLDOWN 5
#define GAMEPLAY_PLAYERS_MOVEMENT_COOLDOWN_SPEED 5

typedef enum gameplay_players_direction_e
{
	GAMEPLAY_PLAYERS_DIRECTION_UP,
	GAMEPLAY_PLAYERS_DIRECTION_RIGHT,
	GAMEPLAY_PLAYERS_DIRECTION_DOWN,
	GAMEPLAY_PLAYERS_DIRECTION_LEFT
} gameplay_players_direction_t;

void gameplay_players_add(int position_x, int position_y, gameplay_players_type_t type);
void gameplay_players_cleanup(void);
void gameplay_players_remove(int position_x, int position_y);
void gameplay_players_update(void);
gameplay_players_player_t *gameplay_players_get_user(void);
void gameplay_players_move(gameplay_players_direction_t direction);
int gameplay_players_amount(void);
gameplay_players_player_t *gameplay_players_get(int index);
void gameplay_players_place_bomb(void);
void gameplay_players_use_item(void);

#endif /* __GAMEPLAY_PLAYERS_H__ */
