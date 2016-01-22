#ifndef __AI_SIMULATION_H__
#define __AI_SIMULATION_H__

void ai_simulation_reset(void);
void ai_simulation_reset_simulated(void);
void ai_simulation_explosion(int position_x, int position_y, char simulated);
int ai_simulation_validate_tile(int position_x_player, int position_y_player, int position_x, int position_y);
void ai_simulation_print(void);

#endif /* __AI_SIMULATION_H__ */
