#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

#include "gameplay.h"
#include "gameplay-players.h"
#include "gameplay-bombs.h"
#include "core.h"

static gameplay_field_t gameplay_field[GAMEPLAY_FIELD_WIDTH * GAMEPLAY_FIELD_HEIGHT];

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
			GAMEPLAY_FIELD(gameplay_field, x, y).item = 0;
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
}

void gameplay_players_initialize(void)
{
	gameplay_players_add(1, 1, GAMEPLAY_PLAYERS_TYPE_USER);
	gameplay_players_add(GAMEPLAY_FIELD_WIDTH - 2, 1, GAMEPLAY_PLAYERS_TYPE_AI);
	gameplay_players_add(1, GAMEPLAY_FIELD_HEIGHT - 2, GAMEPLAY_PLAYERS_TYPE_AI);
	gameplay_players_add(GAMEPLAY_FIELD_WIDTH - 2, GAMEPLAY_FIELD_HEIGHT - 2, GAMEPLAY_PLAYERS_TYPE_AI);
}

void gameplay_cleanup(void)
{
	gameplay_players_cleanup();
	gameplay_bombs_cleanup();
}

int gameplay_get_walkable(int position_x, int position_y)
{
	return (GAMEPLAY_FIELD(gameplay_field, position_x, position_y).type == FLOOR);
}

// also removes the item from the tile
gameplay_items_item_t gameplay_get_item(int position_x, int position_y)
{
	gameplay_items_item_t item = EMPTY;
	
	item = GAMEPLAY_FIELD(gameplay_field, position_x, position_y).item;
	GAMEPLAY_FIELD(gameplay_field, position_x, position_y).item = EMPTY;
	
	return item;
}

void gameplay_destroy(int position_x, int position_y)
{
	if(GAMEPLAY_FIELD(gameplay_field, position_x, position_y).type == DESTRUCTIVE)
	{
		GAMEPLAY_FIELD(gameplay_field, position_x, position_y).type = FLOOR;
		GAMEPLAY_FIELD(gameplay_field, position_x, position_y).item = HEALTH;
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
	}
}

/**
 * This function changes roundly changed values like item timers.
 */
void gameplay_update(void)
{
	gameplay_players_update();
	gameplay_bombs_update();
}

gameplay_field_t *gameplay_get_field(void)
{
	return gameplay_field;
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
