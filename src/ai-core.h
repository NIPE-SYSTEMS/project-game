#ifndef __AI_CORE_H__
#define __AI_CORE_H__

#include "gameplay-players.h"

void ai_core_enable(void);
void ai_core_update(gameplay_players_player_t *player);
void ai_core_cleanup(gameplay_players_player_t *player);

#endif /* __AI_CORE_H__ */
