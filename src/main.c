#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>

// color definitions
#define RENDERING_COLOR_PAIR_WHITE 1
#define RENDERING_COLOR_PAIR_RED 2
#define RENDERING_COLOR_PAIR_GREEN 3
#define RENDERING_COLOR_WHITE COLOR_PAIR(RENDERING_COLOR_PAIR_WHITE)
#define RENDERING_COLOR_RED COLOR_PAIR(RENDERING_COLOR_PAIR_RED)
#define RENDERING_COLOR_GREEN COLOR_PAIR(RENDERING_COLOR_PAIR_GREEN)

// void map_render_tile(unsigned int tile_x, unsigned int tile_y)
// {
// 	unsigned int x = 0;
// 	unsigned int y = 0;
// 	unsigned int x_temp = 0;
// 	unsigned int y_temp = 0;
	
// 	// enable color
// 	switch(map[tile_x][tile_y].tile_type)
// 	{
// 		case TILE_TYPE_WALL:
// 			attron(RENDERING_COLOR_RED);
// 			break;
// 		case TILE_TYPE_DESTRUCTIVE_WALL:
// 			attron(RENDERING_COLOR_GREEN);
// 			break;
// 		case TILE_TYPE_FLOOR:
// 			attron(RENDERING_COLOR_WHITE);
// 			break;
// 	}
	
// 	// render sprite at the correct location:
// 	// loop through the sprite "pixels" and calculate the corresponding position
// 	// on the screen
// 	for(y = 0; y < RENDERING_MAP_TILE_HEIGHT; y++)
// 	{
// 		for(x = 0; x < RENDERING_MAP_TILE_WIDTH; x++)
// 		{
// 			// calculate position of character
// 			x_temp = RENDERING_MAP_OFFSET_X + (tile_x * (RENDERING_MAP_TILE_WIDTH + RENDERING_MAP_TILE_BORDER)) + x;
// 			y_temp = RENDERING_MAP_OFFSET_Y + (tile_y * (RENDERING_MAP_TILE_HEIGHT + RENDERING_MAP_TILE_BORDER)) + y;
			
// 			// output the character at the current position of the sprite
// 			switch(map[tile_x][tile_y].tile_type)
// 			{
// 				case TILE_TYPE_WALL:
// 					mvaddch(y_temp, x_temp, tile_wall[y][x]);
// 					break;
// 				case TILE_TYPE_DESTRUCTIVE_WALL:
// 					mvaddch(y_temp, x_temp, tile_destructive_wall[y][x]);
// 					break;
// 				case TILE_TYPE_FLOOR:
// 					mvaddch(y_temp, x_temp, tile_floor[y][x]);
// 					break;
// 			}
// 		}
// 	}
	
// 	// disable color
// 	switch(map[tile_x][tile_y].tile_type)
// 	{
// 		case TILE_TYPE_WALL:
// 			attroff(RENDERING_COLOR_RED);
// 			break;
// 		case TILE_TYPE_DESTRUCTIVE_WALL:
// 			attroff(RENDERING_COLOR_GREEN);
// 			break;
// 		case TILE_TYPE_FLOOR:
// 			attroff(RENDERING_COLOR_WHITE);
// 			break;
// 	}
// }

#define I_MAX 100

int main(void)
{
	unsigned int i = 0;
	int character = 0;
	
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	start_color();
	use_default_colors();
	init_pair(RENDERING_COLOR_PAIR_RED, COLOR_RED, -1);
	init_pair(RENDERING_COLOR_PAIR_GREEN, COLOR_GREEN, -1);
	init_pair(RENDERING_COLOR_PAIR_WHITE, COLOR_WHITE, -1);
	
	for(i = 0; i < I_MAX; i++)
	{
		clear();
		mvprintw(1, 1, "frame %i of %i", i, I_MAX);
		character = getch();
		if(character != ERR)
		{
			mvprintw(2, 1, "charcode: %i", character);
		}
		move(0, 0);
		refresh();
		usleep(100000);
	}
	
	endwin();
	
	return 0;
}
