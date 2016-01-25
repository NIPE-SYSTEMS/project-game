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
