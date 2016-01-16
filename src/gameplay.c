#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

#include "gameplay.h"
#include "core.h"

static gameplay_field_t gameplay_field[GAMEPLAY_FIELD_WIDTH * GAMEPLAY_FIELD_HEIGHT];
static gameplay_player_t gameplay_player;
static gameplay_keys_t gameplay_keys;

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
			GAMEPLAY_FIELD(x, y).type = DESTRUCTIVE;
			GAMEPLAY_FIELD(x, y).player = 0;
			GAMEPLAY_FIELD(x, y).explosion = 0;
			GAMEPLAY_FIELD(x, y).bomb = 0;
			GAMEPLAY_FIELD(x, y).item = 0;
			GAMEPLAY_FIELD(x, y).timing = 0;
		}
	}
	
	// set outer walls in x dimension
	for(x = 0; x < GAMEPLAY_FIELD_WIDTH; x++)
	{
		GAMEPLAY_FIELD(x, 0).type = WALL;
		GAMEPLAY_FIELD(x, (GAMEPLAY_FIELD_HEIGHT - 1)).type = WALL;
	}
	
	// set outer walls in y dimension
	for(y = 0; y < GAMEPLAY_FIELD_HEIGHT; y++)
	{
		GAMEPLAY_FIELD(0, y).type = WALL;
		GAMEPLAY_FIELD((GAMEPLAY_FIELD_WIDTH - 1), y).type = WALL;
	}
	
	// move player
	GAMEPLAY_FIELD(1, 1).player = 1;
	
	// set walls in the inner field
	for(y = 2; y < GAMEPLAY_FIELD_HEIGHT - 1; y += 2)
	{
		for(x = 2; x < GAMEPLAY_FIELD_WIDTH - 1; x += 2)
		{
			GAMEPLAY_FIELD(x, y).type = WALL;
		}
	}
	
	// remove upper left corner
	GAMEPLAY_FIELD(1, 1).type = FLOOR; // remove wall at (1, 1)
	GAMEPLAY_FIELD(2, 1).type = FLOOR; // remove wall at (2, 1)
	GAMEPLAY_FIELD(1, 2).type = FLOOR; // remove wall at (1, 2)
	
	// remove upper right corner
	GAMEPLAY_FIELD(GAMEPLAY_FIELD_WIDTH - 2, 1).type = FLOOR; // remove wall at (width - 2, 1)
	GAMEPLAY_FIELD(GAMEPLAY_FIELD_WIDTH - 3, 1).type = FLOOR; // remove wall at (width - 3, 1)
	GAMEPLAY_FIELD(GAMEPLAY_FIELD_WIDTH - 2, 2).type = FLOOR; // remove wall at (width - 2, 2)
	
	// remove lower left corner
	GAMEPLAY_FIELD(1, GAMEPLAY_FIELD_HEIGHT - 2).type = FLOOR; // remove wall at (1, height - 2)
	GAMEPLAY_FIELD(2, GAMEPLAY_FIELD_HEIGHT - 2).type = FLOOR; // remove wall at (2, height - 2)
	GAMEPLAY_FIELD(1, GAMEPLAY_FIELD_HEIGHT - 3).type = FLOOR; // remove wall at (1, height - 3)
	
	// remove lower right corner
	GAMEPLAY_FIELD(GAMEPLAY_FIELD_WIDTH - 2, GAMEPLAY_FIELD_HEIGHT - 2).type = FLOOR; // remove wall at (width - 2, height - 2)
	GAMEPLAY_FIELD(GAMEPLAY_FIELD_WIDTH - 3, GAMEPLAY_FIELD_HEIGHT - 2).type = FLOOR; // remove wall at (width - 3, height - 2)
	GAMEPLAY_FIELD(GAMEPLAY_FIELD_WIDTH - 2, GAMEPLAY_FIELD_HEIGHT - 3).type = FLOOR; // remove wall at (width - 2, height - 3)
	
	gameplay_player.health_points = 3;
	gameplay_player.movement_cooldown = 10;
	gameplay_player.position_y = 1;
	gameplay_player.position_x = 1;
	gameplay_player.placeable_bombs = 50; // normal is 1
	gameplay_player.placed_bombs = 0;
	gameplay_player.item = EMPTY;
	gameplay_player.item_usage_time = 0;
	gameplay_player.damage_cooldown = 0;
	return;
}

/**
 * This function resets the gameplay_keys struct after one main loop for 
 * savings in the next round.
 */
void gameplay_key_reset(void)
{
	memset(&gameplay_keys, 0, sizeof(gameplay_keys_t));
	return;
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
			gameplay_keys.key_w = 1;
			break;
		}
		case 'a':
		{
			gameplay_keys.key_a = 1;
			break;
		}
		case 's':
		{
			gameplay_keys.key_s = 1;
			break;
		}
		case 'd':
		{
			gameplay_keys.key_d = 1;
			break;
		}
		case ' ':
		{
			gameplay_keys.key_space = 1;
			break;
		}
		case 'f':
		{
			gameplay_keys.key_f = 1;
			break;
		}
	}
}

/**
 * This function checks the possibility to move the player one field up, if 
 * this is possible it does it.
 */
void gameplay_test_move_up(void)
{
	gameplay_field_t *field_current = &GAMEPLAY_FIELD(gameplay_player.position_x, gameplay_player.position_y);
	gameplay_field_t *field_next = &GAMEPLAY_FIELD(gameplay_player.position_x, gameplay_player.position_y - 1);
	
	if(GAMEPLAY_W_PRESSED && gameplay_player.movement_cooldown == 0 && field_next->type == FLOOR && field_next->bomb == 0)
	{
		field_current->player = 0;
		gameplay_player.position_y--;
		gameplay_player.movement_cooldown = GAMEPLAY_MOVE_COOLDOWN;
		field_next->player = 1;
	}
}

/**
 * This function checks the possibility to move the player one field left, if 
 * this is possible it does it.
 */
void gameplay_test_move_left(void)
{
	gameplay_field_t *field_current = &GAMEPLAY_FIELD(gameplay_player.position_x, gameplay_player.position_y);
	gameplay_field_t *field_next = &GAMEPLAY_FIELD(gameplay_player.position_x - 1, gameplay_player.position_y);
	
	if(GAMEPLAY_A_PRESSED && gameplay_player.movement_cooldown == 0 && field_next->type == FLOOR && field_next->bomb == 0)
	{
		field_current->player = 0;
		gameplay_player.position_x--;
		gameplay_player.movement_cooldown = GAMEPLAY_MOVE_COOLDOWN;
		field_next->player = 1;
	}
}

/**
 * This function checks the possibility to move the player one field down, if 
 * this is possible it does it.
 */
void gameplay_test_move_down(void)
{
	gameplay_field_t *field_current = &GAMEPLAY_FIELD(gameplay_player.position_x, gameplay_player.position_y);
	gameplay_field_t *field_next = &GAMEPLAY_FIELD(gameplay_player.position_x, gameplay_player.position_y + 1);
	
	if(GAMEPLAY_S_PRESSED && gameplay_player.movement_cooldown == 0 && field_next->type == FLOOR && field_next->bomb == 0)
	{
		field_current->player = 0;
		gameplay_player.position_y++;
		gameplay_player.movement_cooldown = GAMEPLAY_MOVE_COOLDOWN;
		field_next->player = 1;
	}
}

/**
 * This function checks the possibility to move the player one field right, if 
 * this is possible it does it.
 */
void gameplay_test_move_right(void)
{
	gameplay_field_t *field_current = &GAMEPLAY_FIELD(gameplay_player.position_x, gameplay_player.position_y);
	gameplay_field_t *field_next = &GAMEPLAY_FIELD(gameplay_player.position_x + 1, gameplay_player.position_y);
	
	if(GAMEPLAY_D_PRESSED && gameplay_player.movement_cooldown == 0 && field_next->type == FLOOR && field_next->bomb == 0)
	{
		field_current->player = 0;
		gameplay_player.position_x++;
		gameplay_player.movement_cooldown = GAMEPLAY_MOVE_COOLDOWN;
		field_next->player = 1;
	}
}

void gameplay_test_place_bomb(void)
{
	gameplay_field_t *field_current = &GAMEPLAY_FIELD(gameplay_player.position_x, gameplay_player.position_y);
	
	if(GAMEPLAY_SPACE_PRESSED && gameplay_player.placeable_bombs - gameplay_player.placed_bombs > 0 && field_current->bomb == 0)
	{
		core_debug("Bomb placed at (%i, %i)", gameplay_player.position_x, gameplay_player.position_y);
		
		field_current->bomb = 1;
		field_current->timing = 50;
		gameplay_player.placed_bombs++;
	}
}

/**
 * This function interprets the pressed buttons and do the following actions.
 */
void gameplay_interpret(void)
{
	gameplay_test_move_up();
	gameplay_test_move_left();
	gameplay_test_move_down();
	gameplay_test_move_right();
	gameplay_test_place_bomb();
	gameplay_test_item_pick_up();
	
	return;
}

/**
 * This function changes roundly changed values like item timers.
 */
void gameplay_buffer(void)
{
	int x = 0;
	int y = 0;
	
	if(gameplay_player.movement_cooldown != 0)
	{
		gameplay_player.movement_cooldown--;
	}
	
	if(gameplay_player.item_usage_time != 0)
	{
		gameplay_player.item_usage_time--;
		if(gameplay_player.item_usage_time == 0)
		{
			gameplay_player.item = EMPTY;
		}
	}
	
	if(gameplay_player.damage_cooldown != 0)
	{
		gameplay_player.damage_cooldown--;
	}
	
	for(y = 0; y < GAMEPLAY_FIELD_HEIGHT; y++)
	{
		for(x = 0; x < GAMEPLAY_FIELD_WIDTH; x++)
		{
			if(GAMEPLAY_FIELD(x, y).bomb == 1)
			{
				GAMEPLAY_FIELD(x, y).timing--;
				if(GAMEPLAY_FIELD(x, y).timing == 0)
				{
					GAMEPLAY_FIELD(x, y).bomb = 0;
					gameplay_explosion(x, y);
				}
			}
			else if(GAMEPLAY_FIELD(x, y).item == 1)
			{
				GAMEPLAY_FIELD(x, y).timing--;
				if(GAMEPLAY_FIELD(x, y).timing == 0)
				{
					GAMEPLAY_FIELD(x, y).item = 0;
				}
			}
			else if(GAMEPLAY_FIELD(x, y).explosion == 1)
			{
				GAMEPLAY_FIELD(x, y).timing--;
				if(GAMEPLAY_FIELD(x, y).timing == 0)
				{
					GAMEPLAY_FIELD(x, y).explosion = 0;
				}
			}
			if(GAMEPLAY_FIELD(x, y).explosion == 1 && GAMEPLAY_FIELD(x, y).player == 1 && gameplay_player.damage_cooldown == 0)
			{
				gameplay_player.health_points--;
				gameplay_player.damage_cooldown = 20;
				if(gameplay_player.health_points == 0)
				{
					//game_over();
				}
			}
		}
	}
}

void gameplay_test_item_pick_up(void)
{
	gameplay_field_t *field_current = &GAMEPLAY_FIELD(gameplay_player.position_x, gameplay_player.position_y);
	
	if(GAMEPLAY_F_PRESSED && field_current->item != EMPTY)
	{
		gameplay_player.item = field_current->item;
		field_current->item = EMPTY;
		gameplay_use_item();
	}
}

void gameplay_use_item(void)
{
	switch(gameplay_player.item)
	{
		case EMPTY:
		{
			return;
			break;
		}
		case HEALTH:
		{
			gameplay_player.health_points++;
			gameplay_player.item = EMPTY;
			break;
		}
		case EXTRA_BOMB:
		{
			gameplay_player.placeable_bombs++;
			gameplay_player.item = EMPTY;
			break;
		}
		case SPEED:
		{
			gameplay_player.movement_cooldown--;
			gameplay_player.item = EMPTY;
			break;
		}
		case SHIELD:
		{
			gameplay_player.damage_cooldown = 100;
			gameplay_player.item = EMPTY;
			break;
		}
	}
}

/**
 * This function simulates a explosion by a bomb.
 */
void gameplay_explosion(int pos_x, int pos_y)
{
	int x = 0;
	int y = 0;
	
	for(y = pos_y; y > 0; y--)
	{
		if(GAMEPLAY_FIELD(pos_x, y).type == WALL)
		{
			break;
		}
		else if(GAMEPLAY_FIELD(pos_x, y).type == DESTRUCTIVE)
		{
			GAMEPLAY_FIELD(pos_x, y).type = FLOOR;
			GAMEPLAY_FIELD(pos_x, y).explosion = 1;
			GAMEPLAY_FIELD(pos_x, y).timing = GAMEPLAY_FIRE_TIME;
			
			//Random Drops!!!
			
			break;
		}
		else if(GAMEPLAY_FIELD(pos_x, y).item != EMPTY)
		{
			GAMEPLAY_FIELD(pos_x, y).item = EMPTY;
			GAMEPLAY_FIELD(pos_x, y).explosion = 1;
			GAMEPLAY_FIELD(pos_x, y).timing = GAMEPLAY_FIRE_TIME;
			break;
		}
		else if(GAMEPLAY_FIELD(pos_x, y).type == FLOOR)
		{
			GAMEPLAY_FIELD(pos_x, y).explosion = 1;
			GAMEPLAY_FIELD(pos_x, y).timing = GAMEPLAY_FIRE_TIME;
		}
	}
	
	for(y = pos_y; y < GAMEPLAY_FIELD_HEIGHT; y++)
	{
		if(GAMEPLAY_FIELD(pos_x, y).type == WALL)
		{
			break;
		}
		else if(GAMEPLAY_FIELD(pos_x, y).type == DESTRUCTIVE)
		{
			GAMEPLAY_FIELD(pos_x, y).type = FLOOR;
			GAMEPLAY_FIELD(pos_x, y).explosion = 1;
			GAMEPLAY_FIELD(pos_x, y).timing = GAMEPLAY_FIRE_TIME;
			GAMEPLAY_FIELD(pos_x, y).item = 6;
			//Random Drops!!!
			
			break;
		}
		else if(GAMEPLAY_FIELD(pos_x, y).item != EMPTY)
		{
			GAMEPLAY_FIELD(pos_x, y).item = EMPTY;
			GAMEPLAY_FIELD(pos_x, y).explosion = 1;
			GAMEPLAY_FIELD(pos_x, y).timing = GAMEPLAY_FIRE_TIME;
			break;
		}
		else if(GAMEPLAY_FIELD(pos_x, y).type == FLOOR)
		{
			GAMEPLAY_FIELD(pos_x, y).explosion = 1;
			GAMEPLAY_FIELD(pos_x, y).timing = GAMEPLAY_FIRE_TIME;
		}
	}
	
	for(x = pos_x; x > 0; x--)
	{
		if(GAMEPLAY_FIELD(x, pos_y).type == WALL)
		{
			break;
		}
		else if(GAMEPLAY_FIELD(x, pos_y).type == DESTRUCTIVE)
		{
			GAMEPLAY_FIELD(x, pos_y).type = FLOOR;
			GAMEPLAY_FIELD(x, pos_y).explosion = 1;
			GAMEPLAY_FIELD(x, pos_y).timing = GAMEPLAY_FIRE_TIME;
			GAMEPLAY_FIELD(x, pos_y).item = 11;
			//Random Drops!!!
			
			break;
		}
		else if(GAMEPLAY_FIELD(x, pos_y).item != EMPTY)
		{
			GAMEPLAY_FIELD(x, pos_y).item = EMPTY;
			GAMEPLAY_FIELD(x, pos_y).explosion = 1;
			GAMEPLAY_FIELD(x, pos_y).timing = GAMEPLAY_FIRE_TIME;
			break;
		}
		else if(GAMEPLAY_FIELD(x, pos_y).type == FLOOR)
		{
			GAMEPLAY_FIELD(x, pos_y).explosion = 1;
			GAMEPLAY_FIELD(x, pos_y).timing = GAMEPLAY_FIRE_TIME;
		}
	}
	
	for(x = pos_x; x < GAMEPLAY_FIELD_WIDTH; x++)
	{
		if(GAMEPLAY_FIELD(x, pos_y).type == WALL)
		{
			break;
		}
		else if(GAMEPLAY_FIELD(x, pos_y).type == DESTRUCTIVE)
		{
			GAMEPLAY_FIELD(x, pos_y).type = FLOOR;
			GAMEPLAY_FIELD(x, pos_y).explosion = 1;
			GAMEPLAY_FIELD(x, pos_y).timing = GAMEPLAY_FIRE_TIME;
			GAMEPLAY_FIELD(x, pos_y).item = 8;
			//Random Drops!!!
			
			break;
		}
		else if(GAMEPLAY_FIELD(x, pos_y).item != EMPTY)
		{
			GAMEPLAY_FIELD(x, pos_y).item = EMPTY;
			GAMEPLAY_FIELD(x, pos_y).explosion = 1;
			GAMEPLAY_FIELD(x, pos_y).timing = GAMEPLAY_FIRE_TIME;
			break;
		}
		else if(GAMEPLAY_FIELD(x, pos_y).type == FLOOR)
		{
			GAMEPLAY_FIELD(x, pos_y).explosion = 1;
			GAMEPLAY_FIELD(x, pos_y).timing = GAMEPLAY_FIRE_TIME;
		}
	}
}

gameplay_field_t *gameplay_get_field(void)
{
	return gameplay_field;
}

gameplay_player_t *gameplay_get_player(void)
{
	return &gameplay_player;
}

/*

								-filling field
								-test_move
								-properties of a map tile (Array von diesem structtyp erstellen für jedes Feld also :
									struct field_properties field[FIELD_HEIGHT][FIELD_WIDTH])
									dadurch wird ein Array an Strukturen erstellt in dem alle Felder existieren
									erhalte dann ein Array an Feldern und kann in diesem direkt die Daten von jedem Feld abspeichern
									map tile type as number or letter for example X for undestroyable walls or something else
								-properties of a player
								-item Array/Struct; (Colors[] ähnlich aus der BMP Aufgabe, also jedem Item eine zahl in dem Array zuordnen und diese dann in dem propertie struct abspeichern)
								-wir brauchen noch Design für den Fall dass Spieler und Bombe gleichzeitSig auf einem Feld befinden
								-items destroyable by bombs
								-field type "item"
								-roundly function to check up and change roundly changed values like item times etc.
								-function "item pick up" for case that a field got an item
								-player damage cooldown
-random drops
-movementcooldown bei speed?!
-damagecooldown bei shield?!


*/
