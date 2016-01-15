#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#include "graphics.h"
#include "gameplay.h"

static char loading_symbol[4] = {'/','-','\\','|'};

static char fields[14][3][5]= {
	
	//Undestroyable field = 0
	{
		{'#','#', '#', '#', '#'},
		{'#','#', '#', '#', '#'},
		{'#','#', '#', '#', '#'}
	},
	//Destroyable field = 1
	{
		{'/','/', '/', '/', '/'},
		{'/','/', '/', '/', '/'},
		{'/','/', '/', '/', '/'}
	},
	//Destroyed field = 2
	{
		{' ',' ', ' ', ' ', ' '},
		{' ',' ', ' ', ' ', ' '},
		{' ',' ', ' ', ' ', ' '}
	},
	//Player = 3
	{
		{' ',' ','O',' ',' '},
		{' ','/','|','\\',' '},
		{' ','/',' ','\\',' '}
	},
	//ENEMY = 4
	{
		{' ',' ','Y',' ',' '},
		{' ','/','|','\\',' '},
		{' ','/',' ','\\',' '}
	},
	//Bomb = 5
	{
		{' ',' ','/','~',' '},
		{'/','#','\\',' ',' '},
		{'\\','_','/',' ',' '}
	},
	//Bomb up = 6
	{
		{'/','-','-','\\','~'},
		{'|','+',' ','|',' '},
		{'\\','_','_','/',' '}
	},
	//Fire = 7
	{
		{' ',' ', ' ', ' ', '+'},
		{'/','\\',' ','/','\\'},
		{'\\','#','#','#', '/'}
	},
	//Heart = 8
	{
		{'/','\\','/','\\',' '},
		{'\\',' ',' ','/',' '},
		{' ','\\','/',' ',' '}
	},
	//Sneakers = 9
	{
		{' ',' ',' ',' ','+'},
		{'|','L','_','_',' '},
		{'L','_','_','_','D'}
	},
	//1 Up = 10 
	{
		{'/','-','-','-','\\'},
		{'-','|', '1','|','-'},
		{' ','|', '+','|',' '}
	},
	//Shield = 11
	{
		{' ','_','_','_',' '},
		{' ','\\','S','/',' '},
		{' ',' ','V',' ',' '}
	},
	//Explosion= 12
	{
		{' ',' ', ' ', ' ', ' '},
		{'/','\\',' ','/','\\'},
		{'\\','#','#','#', '/'}
	},
	//Explosion (second Animation)
	{
		{'/','\\',' ','/','\\'},
		{'|',' ', 'V', ' ', '|'},
		{'\\','#','#','#', '/'}
	}
};
///Note: Explosions are missing!

static int graphics_fieldpos_x = 0;
static int graphics_fieldpos_y = 0;
static int graphics_offset_x = 4;
static int graphics_offset_y = 8;
static int fieldsize_x = GAMEPLAY_FIELD_HEIGHT;
static int fieldsize_y = GAMEPLAY_FIELD_WIDTH;
static int spritesize_x = 5;
static int spritesize_y = 3;
static int animation_counter = 0;

int graphics_create_field(int pos_x, int pos_y, int field)
{
	int i = 0;
	int b = 0;
	for (i = 0; i < spritesize_y; i++)
	{
		for (b = 0; b < spritesize_x; b++)
		{
			mvaddch(pos_x+i, pos_y+b, fields[field][i][b]);
		}
	}
	
	return 0;
}

int graphics_main(void)
{
	gameplay_field_t *gameplay_field = gameplay_get_field();
	gameplay_player_t *gameplay_player = gameplay_get_player();
	
	for(graphics_fieldpos_y = 0; graphics_fieldpos_y < fieldsize_y; graphics_fieldpos_y++)
	{
		for(graphics_fieldpos_x = 0; graphics_fieldpos_x < fieldsize_x; graphics_fieldpos_x++)
		{
			//Creates Field
			graphics_create_field((graphics_fieldpos_x*graphics_offset_x)+graphics_offset_x-spritesize_y, (graphics_fieldpos_y*graphics_offset_y)+graphics_offset_y-spritesize_x, gameplay_field[graphics_fieldpos_y * GAMEPLAY_FIELD_WIDTH + graphics_fieldpos_x].type);
			
			
			//creates Item
			if(gameplay_field[graphics_fieldpos_y * GAMEPLAY_FIELD_WIDTH + graphics_fieldpos_x].item != 0)
			{
				graphics_create_field((graphics_fieldpos_x*graphics_offset_x)+graphics_offset_x-spritesize_y, (graphics_fieldpos_y*graphics_offset_y)+graphics_offset_y-spritesize_x, gameplay_field[graphics_fieldpos_y * GAMEPLAY_FIELD_WIDTH + graphics_fieldpos_x].item);
			}
			
			//creates Fire
			if(gameplay_field[graphics_fieldpos_y * GAMEPLAY_FIELD_WIDTH + graphics_fieldpos_x].explosion == 1)
			{
				graphics_create_field((graphics_fieldpos_x*graphics_offset_x)+graphics_offset_x-spritesize_y, (graphics_fieldpos_y*graphics_offset_y)+graphics_offset_y-spritesize_x, EXPLOSION_SPRITE + animation_counter% 2);
			}
			
			//creates Bomb
			if(gameplay_field[graphics_fieldpos_y * GAMEPLAY_FIELD_WIDTH + graphics_fieldpos_x].bomb == 1)
			{
				graphics_create_field((graphics_fieldpos_x*graphics_offset_x)+graphics_offset_x-spritesize_y, (graphics_fieldpos_y*graphics_offset_y)+graphics_offset_y-spritesize_x, BOMB_SPRITE);
			}
			
		}
	}
	
	animation_counter++;
	
	//creates Hearts
	mvprintw(12,77,"Health:");
	int p = gameplay_player->health_points;
	for(p = gameplay_player->health_points; p > 0; p--)
	{
		graphics_create_field(10 + (graphics_offset_x),69 + (graphics_offset_y *p), HEART_SPRITE);
	}
	
	//creates player
	int i = 0;
	int b = 0;
	for (i = 0; i < spritesize_y; i++)
	{
		for (b = 0; b < spritesize_x; b++)
		{
				if(fields[PLAYER_SPRITE][i][b] != ' ') //Shows underground of the tile the player stands on. 
				{
					mvaddch((gameplay_player->position_y * graphics_offset_x)+graphics_offset_x-spritesize_y+i, (gameplay_player->position_x * graphics_offset_y)+graphics_offset_y-spritesize_x+b, fields[PLAYER_SPRITE][i][b]);
				}
		}
	}
	
	//creates loading indicator
	mvaddch( 36, 99, loading_symbol[animation_counter %4]);
	mvprintw(20,77,"Current item:");
	if(gameplay_player->item == 0)
	{
		graphics_create_field(22,82, 2);
	}
	else
	{
		graphics_create_field(22,82, gameplay_player->item);
	}
	move(0, 0);
	refresh();
	
	return 0;
}
