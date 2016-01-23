#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

#include "gameplay.h"
#include "gameplay-players.h"
#include "gameplay-bombs.h"
#include "core.h"
#include "random-drop.h"
#include "ai-simulation.h"
#include "gameplay-items.h"

static gameplay_field_t gameplay_field[GAMEPLAY_FIELD_WIDTH * GAMEPLAY_FIELD_HEIGHT];
static gameplay_turbo_t gameplay_turbo;

/**
 * This function fills the field array before the game starts with all 
 * neccesary informations.
 */
void gameplay_field_init(void)
{
	int x = 0;
	int y = 0;
	
	core_debug("Initializing field...");
	
	// initialize complete field with default values
	for(y = 0; y < GAMEPLAY_FIELD_HEIGHT; y++)
	{
		for(x = 0; x < GAMEPLAY_FIELD_WIDTH; x++)
		{
			GAMEPLAY_FIELD(gameplay_field, x, y).type = DESTRUCTIVE;
			GAMEPLAY_FIELD(gameplay_field, x, y).ai_position_x = x;
			GAMEPLAY_FIELD(gameplay_field, x, y).ai_position_y = y;
			GAMEPLAY_FIELD(gameplay_field, x, y).ai_pathfinding_number = -1;
			GAMEPLAY_FIELD(gameplay_field, x, y).ai_pathfinding_next = NULL;
			GAMEPLAY_FIELD(gameplay_field, x, y).ai_simulation_walkable = 1;
			GAMEPLAY_FIELD(gameplay_field, x, y).ai_simulation_walkable_simulated = 1;
			GAMEPLAY_FIELD(gameplay_field, x, y).fire = 0;
			GAMEPLAY_FIELD(gameplay_field, x, y).fire_despawn_timer = 0;
		}
	}
	
	// set outer walls in x dimension
	for(x = 0; x < GAMEPLAY_FIELD_WIDTH; x++)
	{
		GAMEPLAY_FIELD(gameplay_field, x, 0).type = WALL;
		GAMEPLAY_FIELD(gameplay_field, x, (GAMEPLAY_FIELD_HEIGHT - 1)).type = WALL;
	}
	
	// set outer walls in y dimension
	for(y = 0; y < GAMEPLAY_FIELD_HEIGHT; y++)
	{
		GAMEPLAY_FIELD(gameplay_field, 0, y).type = WALL;
		GAMEPLAY_FIELD(gameplay_field, (GAMEPLAY_FIELD_WIDTH - 1), y).type = WALL;
	}
	
	// set walls in the inner field
	for(y = 2; y < GAMEPLAY_FIELD_HEIGHT - 1; y += 2)
	{
		for(x = 2; x < GAMEPLAY_FIELD_WIDTH - 1; x += 2)
		{
			GAMEPLAY_FIELD(gameplay_field, x, y).type = WALL;
		}
	}
	
	// remove upper left corner
	GAMEPLAY_FIELD(gameplay_field, 1, 1).type = FLOOR; // remove wall at (1, 1)
	GAMEPLAY_FIELD(gameplay_field, 2, 1).type = FLOOR; // remove wall at (2, 1)
	GAMEPLAY_FIELD(gameplay_field, 1, 2).type = FLOOR; // remove wall at (1, 2)
	
	// remove upper right corner
	GAMEPLAY_FIELD(gameplay_field, GAMEPLAY_FIELD_WIDTH - 2, 1).type = FLOOR; // remove wall at (width - 2, 1)
	GAMEPLAY_FIELD(gameplay_field, GAMEPLAY_FIELD_WIDTH - 3, 1).type = FLOOR; // remove wall at (width - 3, 1)
	GAMEPLAY_FIELD(gameplay_field, GAMEPLAY_FIELD_WIDTH - 2, 2).type = FLOOR; // remove wall at (width - 2, 2)
	
	// remove lower left corner
	GAMEPLAY_FIELD(gameplay_field, 1, GAMEPLAY_FIELD_HEIGHT - 2).type = FLOOR; // remove wall at (1, height - 2)
	GAMEPLAY_FIELD(gameplay_field, 2, GAMEPLAY_FIELD_HEIGHT - 2).type = FLOOR; // remove wall at (2, height - 2)
	GAMEPLAY_FIELD(gameplay_field, 1, GAMEPLAY_FIELD_HEIGHT - 3).type = FLOOR; // remove wall at (1, height - 3)
	
	// remove lower right corner
	GAMEPLAY_FIELD(gameplay_field, GAMEPLAY_FIELD_WIDTH - 2, GAMEPLAY_FIELD_HEIGHT - 2).type = FLOOR; // remove wall at (width - 2, height - 2)
	GAMEPLAY_FIELD(gameplay_field, GAMEPLAY_FIELD_WIDTH - 3, GAMEPLAY_FIELD_HEIGHT - 2).type = FLOOR; // remove wall at (width - 3, height - 2)
	GAMEPLAY_FIELD(gameplay_field, GAMEPLAY_FIELD_WIDTH - 2, GAMEPLAY_FIELD_HEIGHT - 3).type = FLOOR; // remove wall at (width - 2, height - 3)
	
	gameplay_players_initialize();
	
	gameplay_turbo.t = 0;
	gameplay_turbo.u = 0;
	gameplay_turbo.r = 0;
	gameplay_turbo.b = 0;
	gameplay_turbo.o = 0;
}

void gameplay_players_initialize(void)
{
	gameplay_players_add(1, 1, GAMEPLAY_PLAYERS_TYPE_USER);
	gameplay_players_add(GAMEPLAY_FIELD_WIDTH - 2, 1, GAMEPLAY_PLAYERS_TYPE_AI);
	// gameplay_players_add(1, GAMEPLAY_FIELD_HEIGHT - 2, GAMEPLAY_PLAYERS_TYPE_AI);
	// gameplay_players_add(GAMEPLAY_FIELD_WIDTH - 2, GAMEPLAY_FIELD_HEIGHT - 2, GAMEPLAY_PLAYERS_TYPE_AI);
}

void gameplay_cleanup(void)
{
	gameplay_players_cleanup();
	gameplay_bombs_cleanup();
	gameplay_items_cleanup();
}

int gameplay_get_walkable(int position_x, int position_y)
{
	return (GAMEPLAY_FIELD(gameplay_field, position_x, position_y).type == FLOOR);
}

/* also removes the item from the tile
gameplay_items_item_t gameplay_get_item(int position_x, int position_y)
	{
		gameplay_items_item_t item = EMPTY;
	
	item = GAMEPLAY_FIELD(gameplay_field, position_x, position_y).item;
	GAMEPLAY_FIELD(gameplay_field, position_x, position_y).item = EMPTY;
	
	return item;
}*/

void gameplay_destroy(int position_x, int position_y)
{
	random_drop_t drop_list[] =
	{
		{ EMPTY, 0.4 },
		{ HEALTH, 0.1 },
		{ EXTRA_BOMB, 0.2 },
		{FIRE, 0.2},
		{ SPEED, 0.2 },
		{ SHIELD, 0.1 }
	};
	size_t drop_list_amount = sizeof(drop_list) / sizeof(drop_list[0]);
	random_drop_t *picked_drop = NULL;
	
	if(GAMEPLAY_FIELD(gameplay_field, position_x, position_y).type == DESTRUCTIVE)
	{
		GAMEPLAY_FIELD(gameplay_field, position_x, position_y).type = FLOOR;
		picked_drop = random_drop_choose(drop_list, drop_list_amount);
		if(picked_drop != NULL && picked_drop->id != EMPTY)
		{
			gameplay_items_add_item(picked_drop->id, position_x, position_y);
		}
	}
}

/**
 * This function saves the pressed buttons into the gameplay_keys struct for 
 * one main loop.
 */
void gameplay_key(char gameplay_pressed_key)
{
	switch(gameplay_pressed_key)
	{
		case 'w':
		{
			gameplay_players_move(GAMEPLAY_PLAYERS_DIRECTION_UP);
			break;
		}
		case 'd':
		{
			gameplay_players_move(GAMEPLAY_PLAYERS_DIRECTION_RIGHT);
			break;
		}
		case 's':
		{
			gameplay_players_move(GAMEPLAY_PLAYERS_DIRECTION_DOWN);
			break;
		}
		case 'a':
		{
			gameplay_players_move(GAMEPLAY_PLAYERS_DIRECTION_LEFT);
			break;
		}
		case ' ':
		{
			gameplay_players_place_bomb();
			break;
		}
		case 'f':
		{
			gameplay_players_use_item();
			break;
		}
		case 't':
		{
			gameplay_turbo.t =1;
			break;
		}
		case 'u':
		{
			if(gameplay_turbo.t == 1)
			{
				gameplay_turbo.u = 1;
			}
			break;
		}
		case 'r':
		{
			if(gameplay_turbo.u == 1)
			{
				gameplay_turbo.r = 1;
			}
			break;
		}
		case 'b':
		{
			if(gameplay_turbo.r == 1)
			{
				gameplay_turbo.b = 1;
			}
			break;
		}
		case 'o':
		{
			if(gameplay_turbo.b == 1)
			{
				gameplay_turbo.o = 1;
				gameplay_turbo_activated();
			}
			break;
		}
	}
}


void gameplay_turbo_activated()
{
	gameplay_players_player_t *player = NULL;
	player = gameplay_players_get_user();
	player->movement_cooldown_initial = 1;
	player->health_points = 5;
	player->placeable_bombs = 10;
	player->explosion_size = 9;
	player->damage_cooldown_initial = 100;
	animation_turbo_activated = 1;
	
	gameplay_turbo.t = 0;
	gameplay_turbo.u = 0;
	gameplay_turbo.r = 0;
	gameplay_turbo.b = 0;
	gameplay_turbo.o = 0;
}

void gameplay_reset_fire(void)
{
	int x = 0;
	int y = 0;
	
	for(y = 0; y < GAMEPLAY_FIELD_HEIGHT; y++)
	{
		for(x = 0; x < GAMEPLAY_FIELD_WIDTH; x++)
		{
			if(GAMEPLAY_FIELD(gameplay_field,x,y).fire == 1)
			{
				GAMEPLAY_FIELD(gameplay_field, x, y).fire_despawn_timer--;
				gameplay_players_harm(x,y);
				if(GAMEPLAY_FIELD(gameplay_field, x, y).fire_despawn_timer == 0)
				{
					GAMEPLAY_FIELD(gameplay_field, x, y).fire = 0;
				}
			}
		}
	}
}

/**
 * This function changes roundly changed values like item timers.
 */
void gameplay_update(void)
{
	gameplay_reset_fire();
	gameplay_players_update();
	gameplay_bombs_update();
	gameplay_players_ai_update();
	gameplay_items_item_update();
}

gameplay_field_t *gameplay_get_field(void)
{
	return gameplay_field;
}

void gameplay_set_fire(int position_x, int position_y)
{
	if(GAMEPLAY_FIELD(gameplay_field, position_x, position_y).type != FLOOR)
	{
		return;
	}
	
	GAMEPLAY_FIELD(gameplay_field, position_x, position_y).fire = 1;
	GAMEPLAY_FIELD(gameplay_field, position_x, position_y).fire_despawn_timer = GAMEPLAY_FIRE_DESPAWN;
}

int gameplay_get_fire(int position_x, int position_y)
{
	if(GAMEPLAY_FIELD(gameplay_field, position_x, position_y).fire == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
