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
