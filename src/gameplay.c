#include <stdlib.h>
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
	int y = 0;
	int x = 0;
	/*
	while(y < GAMEPLAY_FIELD_HEIGHT)
	{
		x = 0;
		while(x < GAMEPLAY_FIELD_WIDTH)
		{
			if (y == 0 || x == 0 || y == GAMEPLAY_FIELD_HEIGHT - 1 || x == GAMEPLAY_FIELD_WIDTH - 1)
			{
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].type = 0;//WALL;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].player = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].explosion = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].bomb = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].item = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].timing = 0;
			}
			else if((y == 1 && x == 2) || (y == 2 && x == 1) || (y == 1 && x == GAMEPLAY_FIELD_WIDTH - 3) || (y == 1 && x == GAMEPLAY_FIELD_WIDTH - 2) || (y == 2 && x == GAMEPLAY_FIELD_WIDTH - 2) || (y == GAMEPLAY_FIELD_HEIGHT - 2 && x == GAMEPLAY_FIELD_WIDTH - 3) || (y == GAMEPLAY_FIELD_HEIGHT - 3 && x == GAMEPLAY_FIELD_WIDTH - 2) || (y == GAMEPLAY_FIELD_HEIGHT - 2 && x == GAMEPLAY_FIELD_WIDTH - 2) || (y == GAMEPLAY_FIELD_HEIGHT - 3 && x == 1) || (y == GAMEPLAY_FIELD_HEIGHT - 2 && x == 1) || (y == GAMEPLAY_FIELD_HEIGHT - 2 && x == 2))
			{
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].type = 2; //FLOOR;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].player = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].explosion = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].bomb = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].item = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].timing = 0;
			}
			else if((y == 1 && x == 1))
			{
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].type = 2; //FLOOR;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].player = 1;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].explosion = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].bomb = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].item = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].timing = 0;
			}
			else if(y%2 == 0)
			{
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].type = 1; //DESTRUCTIVE;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].player = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].explosion = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].bomb = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].item = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].timing = 0;
			}
			else if(y%2 == 1)
			{
				if(x%2 == 0)
				{
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].type = 0; //DESTRUCTIVE;
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].player = 0;
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].explosion = 0;
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].bomb = 0;
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].item = 0;
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].timing = 0;
				}
				else if (x % 2 != 0)
				{
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].type = 1; //WALL;
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].player = 0;
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].explosion = 0;
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].bomb = 0;
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].item = 0;
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].timing = 0;
				}
			}

			x++;
		}
		y++;
	}
	*/
	for(y = 0; y < GAMEPLAY_FIELD_HEIGHT; y++)
	{
		for(x = 0; x < GAMEPLAY_FIELD_WIDTH; x++)
		{
			if (y == 0 || x == 0 || y == GAMEPLAY_FIELD_HEIGHT - 1 || x == GAMEPLAY_FIELD_WIDTH - 1)
			{
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].type = WALL;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].player = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].explosion = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].bomb = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].item = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].timing = 0;
			}
			else if((y == 1 && x == 2) || (y == 2 && x == 1) || (y == 1 && x == GAMEPLAY_FIELD_WIDTH - 3) || (y == 1 && x == GAMEPLAY_FIELD_WIDTH - 2) || (y == 2 && x == GAMEPLAY_FIELD_WIDTH - 2) || (y == GAMEPLAY_FIELD_HEIGHT - 2 && x == GAMEPLAY_FIELD_WIDTH - 3) || (y == GAMEPLAY_FIELD_HEIGHT - 3 && x == GAMEPLAY_FIELD_WIDTH - 2) || (y == GAMEPLAY_FIELD_HEIGHT - 2 && x == GAMEPLAY_FIELD_WIDTH - 2) || (y == GAMEPLAY_FIELD_HEIGHT - 3 && x == 1) || (y == GAMEPLAY_FIELD_HEIGHT - 2 && x == 1) || (y == GAMEPLAY_FIELD_HEIGHT - 2 && x == 2))
			{
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].type = FLOOR;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].player = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].explosion = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].bomb = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].item = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].timing = 0;
			}
			else if((y == 1 && x == 1))
			{
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].type = FLOOR;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].player = 1;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].explosion = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].bomb = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].item = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].timing = 0;
			}
			else if(y%2 == 0 && x%2 == 0)
			{
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].type = WALL;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].player = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].explosion = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].bomb = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].item = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].timing = 0;
			}
			else
			{
				
				
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].type = DESTRUCTIVE;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].player = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].explosion = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].bomb = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].item = 0;
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].timing = 0;
				
				/*else if (x % 2 == 1)
				{
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].type = DESTRUCTIVE;
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].player = 0;
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].explosion = 0;
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].bomb = 0;
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].item = 0;
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].timing = 0;
				}*/
			}
		}
	}
	
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
	gameplay_keys.key_w = 0;
	gameplay_keys.key_a = 0;
	gameplay_keys.key_s = 0;
	gameplay_keys.key_d = 0;
	gameplay_keys.key_space = 0;
	gameplay_keys.key_f = 0;
	return;
}

/**
 * This function saves the pressed buttons into the gameplay_keys struct for 
 * one main loop.SWITCH!!
 */
void gameplay_key(char gameplay_pressed_key)
{
	
	if(gameplay_pressed_key == 'w')
	{
		gameplay_keys.key_w = 1;
	}
	else if(gameplay_pressed_key == 'a')
	{
		gameplay_keys.key_a = 1;
	}
	else if(gameplay_pressed_key == 's')
	{
		gameplay_keys.key_s = 1;
	}
	else if(gameplay_pressed_key == 'd')
	{
		gameplay_keys.key_d = 1;
	}
	else if(gameplay_pressed_key == ' ')
	{
		gameplay_keys.key_space = 1;
	}
	else if(gameplay_pressed_key == 'f')
	{
		gameplay_keys.key_f = 1;
	}
	return;
}

/**
 * This function checks the possibility to move the player one field up, if 
 * this is possible it does it.
 */
void gameplay_test_move_up(void)
{
	if (gameplay_keys.key_w == 0 || gameplay_player.movement_cooldown != 0)
	{
		return;
	}
	else if(gameplay_keys.key_w == 1 && gameplay_field[(gameplay_player.position_y - 1) * GAMEPLAY_FIELD_WIDTH + gameplay_player.position_x].type == FLOOR && gameplay_field[(gameplay_player.position_y - 1) * GAMEPLAY_FIELD_WIDTH + gameplay_player.position_x].bomb == 0)
	{
		gameplay_field[gameplay_player.position_y * GAMEPLAY_FIELD_WIDTH + gameplay_player.position_x].player = 0;
		gameplay_player.position_y--;
		gameplay_player.movement_cooldown = GAMEPLAY_MOVE_COOLDOWN;
		gameplay_field[gameplay_player.position_y * GAMEPLAY_FIELD_WIDTH + gameplay_player.position_x].player = 1;
	}
	return;
}

/**
 * This function checks the possibility to move the player one field left, if 
 * this is possible it does it.
 */
void gameplay_test_move_left(void)
{
	if (gameplay_keys.key_a == 0 || gameplay_player.movement_cooldown != 0)
	{
		return;
	}
	else if(gameplay_keys.key_a == 1 && gameplay_field[gameplay_player.position_y * GAMEPLAY_FIELD_WIDTH + (gameplay_player.position_x - 1)].type == FLOOR && gameplay_field[gameplay_player.position_y * GAMEPLAY_FIELD_WIDTH + (gameplay_player.position_x - 1)].bomb == 0)
	{
		gameplay_field[gameplay_player.position_y * GAMEPLAY_FIELD_WIDTH + gameplay_player.position_x].player = 0;
		gameplay_player.position_x--;
		gameplay_player.movement_cooldown = GAMEPLAY_MOVE_COOLDOWN;
		gameplay_field[gameplay_player.position_y * GAMEPLAY_FIELD_WIDTH + gameplay_player.position_x].player = 1;
	}
	return;
}

/**
 * This function checks the possibility to move the player one field down, if 
 * this is possible it does it.
 */
void gameplay_test_move_down(void)
{
	if (gameplay_keys.key_s == 0 || gameplay_player.movement_cooldown != 0)
	{
		return;
	}
	else if(gameplay_keys.key_s == 1 && gameplay_field[(gameplay_player.position_y + 1) * GAMEPLAY_FIELD_WIDTH + gameplay_player.position_x].type == FLOOR && gameplay_field[(gameplay_player.position_y + 1) * GAMEPLAY_FIELD_WIDTH + gameplay_player.position_x].bomb == 0)
	{
		
		gameplay_field[gameplay_player.position_y * GAMEPLAY_FIELD_WIDTH + gameplay_player.position_x].player = 0;
		gameplay_player.position_y++;
		gameplay_player.movement_cooldown = GAMEPLAY_MOVE_COOLDOWN;
		gameplay_field[gameplay_player.position_y * GAMEPLAY_FIELD_WIDTH + gameplay_player.position_x].player = 1;
	}
	return;
}

/**
 * This function checks the possibility to move the player one field right, if 
 * this is possible it does it.
 */
void gameplay_test_move_right(void)
{
	if (gameplay_keys.key_d == 0 || gameplay_player.movement_cooldown != 0)
	{
		return;
	}
	else if(gameplay_keys.key_d == 1 && gameplay_field[gameplay_player.position_y * GAMEPLAY_FIELD_WIDTH + (gameplay_player.position_x + 1)].type == FLOOR && gameplay_field[gameplay_player.position_y * GAMEPLAY_FIELD_WIDTH + (gameplay_player.position_x + 1)].bomb == 0)
	{
		gameplay_field[gameplay_player.position_y * GAMEPLAY_FIELD_WIDTH + gameplay_player.position_x].player = 0;
		gameplay_player.position_x++;
		gameplay_player.movement_cooldown = GAMEPLAY_MOVE_COOLDOWN;
		gameplay_field[gameplay_player.position_y * GAMEPLAY_FIELD_WIDTH + gameplay_player.position_x].player = 1;
	}
	return;
}

void gameplay_test_place_bomb(void)
{
	if (gameplay_keys.key_space == 0 || (gameplay_player.placeable_bombs - gameplay_player.placed_bombs) == 0)
	{
		return;
	}
	else if(gameplay_keys.key_space == 1 && gameplay_field[gameplay_player.position_y * GAMEPLAY_FIELD_WIDTH + gameplay_player.position_x].bomb == 0)
	{
		gameplay_field[gameplay_player.position_y * GAMEPLAY_FIELD_WIDTH + gameplay_player.position_x].bomb = 1;
		gameplay_field[gameplay_player.position_y * GAMEPLAY_FIELD_WIDTH + gameplay_player.position_x].timing = 50;
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
	if (gameplay_player.movement_cooldown != 0)
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
	
	int y = 0;
	int x = 0;
	while(y < GAMEPLAY_FIELD_HEIGHT)
	{
		x = 0;
		while(x < GAMEPLAY_FIELD_WIDTH)
		{
			if(gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].bomb == 1)
			{
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].timing--;
				if(gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].timing == 0)
				{
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].bomb = 0;
					gameplay_explosion(y,x);
				}
			}
			else if(gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].item == 1)
			{
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].timing--;
				if(gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].timing == 0)
				{
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].item = 0;
				}
			}
			else if(gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].explosion == 1)
			{
				gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].timing--;
				if(gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].timing == 0)
				{
					gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].explosion = 0;
				}
			}
			if(gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].explosion == 1 && gameplay_field[y * GAMEPLAY_FIELD_WIDTH + x].player == 1 && gameplay_player.damage_cooldown == 0)
			{
				gameplay_player.health_points--;
				gameplay_player.damage_cooldown = 20;
				if(gameplay_player.health_points == 0)
				{
					//game_over();
				}
			}
			x++;
		}
		y++;
	}
	return;
}

void gameplay_test_item_pick_up(void)
{
	if(gameplay_keys.key_f == 0)
	{
		return;
	}
	else if(gameplay_keys.key_f == 1 && gameplay_field[gameplay_player.position_y * GAMEPLAY_FIELD_WIDTH + gameplay_player.position_x].item != EMPTY)
	{
		gameplay_player.item = gameplay_field[gameplay_player.position_y * GAMEPLAY_FIELD_WIDTH + gameplay_player.position_x].item;
		gameplay_field[gameplay_player.position_y * GAMEPLAY_FIELD_WIDTH + gameplay_player.position_x].item = EMPTY;
		gameplay_use_item();
	}
	return;
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
void gameplay_explosion(int y, int x)
{
	int running;
	running = y;
	while(running > 0)
	{
		if(gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].type == WALL)
		{
			break;
		}
		else if(gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].type == DESTRUCTIVE)
		{
			gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].type = FLOOR;
			gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].explosion = 1;
			gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].timing = GAMEPLAY_FIRE_TIME;
			
			//Random Drops!!!
			
			break;
		}
		else if(gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].item != EMPTY)
		{
			gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].item = EMPTY;
			gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].explosion = 1;
			gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].timing = GAMEPLAY_FIRE_TIME;
			break;
		}
		else if(gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].type == FLOOR)
		{
			gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].explosion = 1;
			gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].timing = GAMEPLAY_FIRE_TIME;
		}
		running--;
	}
	
	running = y;
	while(running < GAMEPLAY_FIELD_HEIGHT)
	{
		if(gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].type == WALL)
		{
			break;
		}
		else if(gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].type == DESTRUCTIVE)
		{
			gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].type = FLOOR;
			gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].explosion = 1;
			gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].timing = GAMEPLAY_FIRE_TIME;
			gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].item = 6;
			//Random Drops!!!
			
			break;
		}
		else if(gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].item != EMPTY)
		{
			gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].item = EMPTY;
			gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].explosion = 1;
			gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].timing = GAMEPLAY_FIRE_TIME;
			break;
		}
		else if(gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].type == FLOOR)
		{
			gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].explosion = 1;
			gameplay_field[running * GAMEPLAY_FIELD_WIDTH + x].timing = GAMEPLAY_FIRE_TIME;
		}
		running++;
	}
	
	running = x;
	while(running > 0)
	{
		if(gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].type == WALL)
		{
			break;
		}
		else if(gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].type == DESTRUCTIVE)
		{
			gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].type = FLOOR;
			gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].explosion = 1;
			gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].timing = GAMEPLAY_FIRE_TIME;
			gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].item = 11;
			//Random Drops!!!
			
			break;
		}
		else if(gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].item != EMPTY)
		{
			gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].item = EMPTY;
			gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].explosion = 1;
			gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].timing = GAMEPLAY_FIRE_TIME;
			break;
		}
		else if(gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].type == FLOOR)
		{
			gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].explosion = 1;
			gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].timing = GAMEPLAY_FIRE_TIME;
		}
		running--;
	}
	
	running = x;
	while(running < GAMEPLAY_FIELD_WIDTH)
	{
		if(gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].type == WALL)
		{
			break;
		}
		else if(gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].type == DESTRUCTIVE)
		{
			gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].type = FLOOR;
			gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].explosion = 1;
			gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].timing = GAMEPLAY_FIRE_TIME;
			gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].item = 8;
			//Random Drops!!!
			
			break;
		}
		else if(gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].item != EMPTY)
		{
			gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].item = EMPTY;
			gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].explosion = 1;
			gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].timing = GAMEPLAY_FIRE_TIME;
			break;
		}
		else if(gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].type == FLOOR)
		{
			gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].explosion = 1;
			gameplay_field[y * GAMEPLAY_FIELD_WIDTH + running].timing = GAMEPLAY_FIRE_TIME;
		}
		running++;
	}
	return;
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