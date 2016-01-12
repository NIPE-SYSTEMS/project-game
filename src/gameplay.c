#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#include "gameplay.h"

/**
 * This function fills the field array before the game starts with all 
 * neccesary informations.
 */
void gameplay_field_init()
{
	int h = 0;
	int w = 0;
	while(h < GAMEPLAY_FIELD_HEIGHT)
	{
		w = 0;
		while(w < GAMEPLAY_FIELD_WIDTH)
		{
			if (h == 0 || w == 0)
			{
				gameplay_field[h][w].type = WALL;
				gameplay_field[h][w].player = 0;
				gameplay_field[h][w].explosion = 0;
				gameplay_field[h][w].bomb = 0;
				gameplay_field[h][w].item = 0;
				gameplay_field[h][w].timing = 0;
			}
			else if((h == 1 && w == 2) || (h == 2 && w == 1) || (h == 1 && w == GAMEPLAY_FIELD_WIDTH - 3) || (h == 1 && w == GAMEPLAY_FIELD_WIDTH - 2) || (h == 2 && w == GAMEPLAY_FIELD_WIDTH - 2) || (h == GAMEPLAY_FIELD_HEIGHT - 2 && w == GAMEPLAY_FIELD_WIDTH - 3) || (h == GAMEPLAY_FIELD_HEIGHT - 3 && w == GAMEPLAY_FIELD_WIDTH - 2) || (h == GAMEPLAY_FIELD_HEIGHT - 2 && w == GAMEPLAY_FIELD_WIDTH - 2) || (h == GAMEPLAY_FIELD_HEIGHT - 3 && w == 1) || (h == GAMEPLAY_FIELD_HEIGHT - 2 && w == 1) || (h == GAMEPLAY_FIELD_HEIGHT - 2 && w == 2))
			{
				gameplay_field[h][w].type = FLOOR;
				gameplay_field[h][w].player = 0;
				gameplay_field[h][w].explosion = 0;
				gameplay_field[h][w].bomb = 0;
				gameplay_field[h][w].item = 0;
				gameplay_field[h][w].timing = 0;
			}
			else if((h == 1 && w == 1))
			{
				gameplay_field[h][w].type = FLOOR;
				gameplay_field[h][w].player = 1;
				gameplay_field[h][w].explosion = 0;
				gameplay_field[h][w].bomb = 0;
				gameplay_field[h][w].item = 0;
				gameplay_field[h][w].timing = 0;
			}
			else if(h%2 == 0)
			{
				gameplay_field[h][w].type = DESTRUCTIVE;
				gameplay_field[h][w].player = 0;
				gameplay_field[h][w].explosion = 0;
				gameplay_field[h][w].bomb = 0;
				gameplay_field[h][w].item = 0;
				gameplay_field[h][w].timing = 0;
			}
			else if(h%2 == 1)
			{
				if(w%2 == 0)
				{
					gameplay_field[h][w].type = DESTRUCTIVE;
					gameplay_field[h][w].player = 0;
					gameplay_field[h][w].explosion = 0;
					gameplay_field[h][w].bomb = 0;
					gameplay_field[h][w].item = 0;
					gameplay_field[h][w].timing = 0;
				}
				else if (w % 2 == 1)
				{
					gameplay_field[h][w].type = WALL;
					gameplay_field[h][w].player = 0;
					gameplay_field[h][w].explosion = 0;
					gameplay_field[h][w].bomb = 0;
					gameplay_field[h][w].item = 0;
					gameplay_field[h][w].timing = 0;
				}
			}

			w++;
		}
		h++;
	}
	
	gameplay_player.health_points = 1;
	gameplay_player.movement_cooldown = 10;
	gameplay_player.position_height = 1;
	gameplay_player.position_width = 1;
	gameplay_player.placeable_bombs = 1;
	gameplay_player.placed_bombs = 0;
	gameplay_player.item = 0;
	gameplay_player.item_usage_time = 0;
	gameplay_player.damage_cooldown = 0;
	return;
}

/**
 * This function resets the gameplay_keys struct after one main loop for 
 * savings in the next round.
 */
void gameplay_key_reset()
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
void gameplay_test_move_up()
{
	if (gameplay_keys.key_w == 0 || gameplay_player.movement_cooldown != 0)
	{
		return;
	}
	else if(gameplay_keys.key_w == 1 && gameplay_field[gameplay_player.position_height + 1][gameplay_player.position_width].type == FLOOR && gameplay_field[gameplay_player.position_height + 1][gameplay_player.position_width].bomb == 0)
	{
		gameplay_field[gameplay_player.position_height][gameplay_player.position_width].player = 0;
		gameplay_player.position_height--;
		gameplay_player.movement_cooldown = GAMEPLAY_MOVE_COOLDOWN;
		gameplay_field[gameplay_player.position_height][gameplay_player.position_width].player = 1;
	}
	return;
}

/**
 * This function checks the possibility to move the player one field left, if 
 * this is possible it does it.
 */
void gameplay_test_move_left()
{
	if (gameplay_keys.key_a == 0 || gameplay_player.movement_cooldown != 0)
	{
		return;
	}
	else if(gameplay_keys.key_a == 1 && gameplay_field[gameplay_player.position_height][gameplay_player.position_width - 1].type == FLOOR && gameplay_field[gameplay_player.position_height][gameplay_player.position_width - 1].bomb == 0)
	{
		gameplay_field[gameplay_player.position_height][gameplay_player.position_width].player = 0;
		gameplay_player.position_width--;
		gameplay_player.movement_cooldown = GAMEPLAY_MOVE_COOLDOWN;
		gameplay_field[gameplay_player.position_height][gameplay_player.position_width].player = 1;
	}
	return;
}

/**
 * This function checks the possibility to move the player one field down, if 
 * this is possible it does it.
 */
void gameplay_test_move_down()
{
	if (gameplay_keys.key_s == 0 || gameplay_player.movement_cooldown != 0)
	{
		return;
	}
	else if(gameplay_keys.key_s == 1 && gameplay_field[gameplay_player.position_height - 1][gameplay_player.position_width].type == FLOOR && gameplay_field[gameplay_player.position_height - 1][gameplay_player.position_width].bomb == 0)
	{
		gameplay_field[gameplay_player.position_height][gameplay_player.position_width].player = 0;
		gameplay_player.position_height++;
		gameplay_player.movement_cooldown = GAMEPLAY_MOVE_COOLDOWN;
		gameplay_field[gameplay_player.position_height][gameplay_player.position_width].player = 1;
	}
	return;
}

/**
 * This function checks the possibility to move the player one field right, if 
 * this is possible it does it.
 */
void gameplay_test_move_right()
{
	if (gameplay_keys.key_d == 0 || gameplay_player.movement_cooldown != 0)
	{
		return;
	}
	else if(gameplay_keys.key_d == 1 && gameplay_field[gameplay_player.position_height][gameplay_player.position_width + 1].type == FLOOR && gameplay_field[gameplay_player.position_height][gameplay_player.position_width + 1].bomb == 0)
	{
		gameplay_field[gameplay_player.position_height][gameplay_player.position_width].player = 0;
		gameplay_player.position_width++;
		gameplay_player.movement_cooldown = GAMEPLAY_MOVE_COOLDOWN;
		gameplay_field[gameplay_player.position_height][gameplay_player.position_width].player = 1;
	}
	return;
}

void gameplay_test_place_bomb()
{
	if (gameplay_keys.key_space == 0 || (gameplay_player.placeable_bombs - gameplay_player.placed_bombs == 0))
	{
		return;
	}
	else if(gameplay_keys.key_space == 1 && gameplay_field[gameplay_player.position_height][gameplay_player.position_width].bomb == 0)
	{
		gameplay_field[gameplay_player.position_height][gameplay_player.position_width].bomb = 1;
		gameplay_player.placed_bombs++;
	}
}

/**
 * This function interprets the pressed buttons and do the following actions.
 */
void gameplay_interpret()
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
void gameplay_buffer()
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
	int h = 0;
	int w = 0;
	while(h < GAMEPLAY_FIELD_HEIGHT)
	{
		w = 0;
		while(w < GAMEPLAY_FIELD_WIDTH)
		{
			if(gameplay_field[h][w].bomb == 1)
			{
				gameplay_field[h][w].timing--;
				if(gameplay_field[h][w].timing == 0)
				{
					gameplay_field[h][w].bomb = 0;
					gameplay_explosion(h,w);
				}
			}
			else if(gameplay_field[h][w].item == 1)
			{
				gameplay_field[h][w].timing--;
				if(gameplay_field[h][w].timing == 0)
				{
					gameplay_field[h][w].item = 0;
				}
			}
			else if(gameplay_field[h][w].explosion == 1)
			{
				gameplay_field[h][w].timing--;
				if(gameplay_field[h][w].timing == 0)
				{
					gameplay_field[h][w].explosion = 0;
				}
			}
			if(gameplay_field[h][w].explosion == 1 && gameplay_field[h][w].player == 1 && gameplay_player.damage_cooldown == 0)
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
	return;
}

void gameplay_test_item_pick_up()
{
	if(gameplay_keys.key_f == 0)
	{
		return;
	}
	else if(gameplay_keys.key_f == 1 && gameplay_field[gameplay_player.position_height][gameplay_player.position_width].item != EMPTY)
	{
		gameplay_player.item = gameplay_field[gameplay_player.position_height][gameplay_player.position_width].item;
		gameplay_field[gameplay_player.position_height][gameplay_player.position_width].item = EMPTY;
		gameplay_use_item();
	}
	return;
}

void gameplay_use_item()
{
	if(gameplay_player.item == EMPTY)
	{
		return;
	}
	else if(gameplay_player.item == HEALTH)
	{
		gameplay_player.health_points++;
		gameplay_player.item = EMPTY;
	}
	else if(gameplay_player.item == EXTRA_BOMB)
	{
		gameplay_player.placeable_bombs++;
		gameplay_player.item = EMPTY;
	}
	else if(gameplay_player.item == SPEED)
	{
		gameplay_player.movement_cooldown--;
		gameplay_player.item = EMPTY;
	}
	else if(gameplay_player.item == SHIELD)
	{
		gameplay_player.damage_cooldown = 100;
		gameplay_player.item = EMPTY;
	}
	return;
}

/**
 * This function simulates a explosion by a bomb.
 */
void gameplay_explosion(int height, int width)
{
	int running;
	running = height;
	while(running > 0)
	{
		if(gameplay_field[running][width].type == WALL)
		{
			break;
		}
		else if(gameplay_field[running][width].type == DESTRUCTIVE)
		{
			gameplay_field[running][width].type = FLOOR;
			
			//Random Drops!!!
			
			break;
		}
		else if(gameplay_field[running][width].item != EMPTY)
		{
			gameplay_field[running][width].item = EMPTY;
			gameplay_field[running][width].explosion = 1;
			gameplay_field[running][width].timing = GAMEPLAY_FIRE_TIME;
			break;
		}
		else if(gameplay_field[running][width].type == FLOOR)
		{
			gameplay_field[running][width].explosion = 1;
			gameplay_field[running][width].timing = GAMEPLAY_FIRE_TIME;
		}
		running--;
	}
	
	running = height;
	while(running < GAMEPLAY_FIELD_HEIGHT)
	{
		if(gameplay_field[running][width].type == WALL)
		{
			break;
		}
		else if(gameplay_field[running][width].type == DESTRUCTIVE)
		{
			gameplay_field[running][width].type = FLOOR;
			
			//Random Drops!!!
			
			break;
		}
		else if(gameplay_field[running][width].item != EMPTY)
		{
			gameplay_field[running][width].item = EMPTY;
			gameplay_field[running][width].explosion = 1;
			gameplay_field[running][width].timing = GAMEPLAY_FIRE_TIME;
			break;
		}
		else if(gameplay_field[running][width].type == FLOOR)
		{
			gameplay_field[running][width].explosion = 1;
			gameplay_field[running][width].timing = GAMEPLAY_FIRE_TIME;
		}
		running++;
	}
	
	running = width;
	while(running > 0)
	{
		if(gameplay_field[height][running].type == WALL)
		{
			break;
		}
		else if(gameplay_field[height][running].type == DESTRUCTIVE)
		{
			gameplay_field[height][running].type = FLOOR;
			
			//Random Drops!!!
			
			break;
		}
		else if(gameplay_field[height][running].item != EMPTY)
		{
			gameplay_field[height][running].item = EMPTY;
			gameplay_field[height][running].explosion = 1;
			gameplay_field[height][running].timing = GAMEPLAY_FIRE_TIME;
			break;
		}
		else if(gameplay_field[height][running].type == FLOOR)
		{
			gameplay_field[height][running].explosion = 1;
			gameplay_field[height][running].timing = GAMEPLAY_FIRE_TIME;
		}
		running--;
	}
	
	running = width;
	while(running < GAMEPLAY_FIELD_WIDTH)
	{
		if(gameplay_field[height][running].type == WALL)
		{
			break;
		}
		else if(gameplay_field[height][running].type == DESTRUCTIVE)
		{
			gameplay_field[height][running].type = FLOOR;
			
			//Random Drops!!!
			
			break;
		}
		else if(gameplay_field[height][running].item != EMPTY)
		{
			gameplay_field[height][running].item = EMPTY;
			gameplay_field[height][running].explosion = 1;
			gameplay_field[height][running].timing = GAMEPLAY_FIRE_TIME;
			break;
		}
		else if(gameplay_field[height][running].type == FLOOR)
		{
			gameplay_field[height][running].explosion = 1;
			gameplay_field[height][running].timing = GAMEPLAY_FIRE_TIME;
		}
		running++;
	}
	return;
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