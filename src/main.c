#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <time.h>

// map size
#define MAP_WIDTH 10
#define MAP_HEIGHT 10

// some irrelevant values
#define TILE_TYPE_WALL 1
#define TILE_TYPE_DESTRUCTIVE_WALL 2
#define TILE_TYPE_FLOOR 3

// rendering definitions
#define RENDERING_MAP_OFFSET_X 3
#define RENDERING_MAP_OFFSET_Y 1
#define RENDERING_MAP_TILE_WIDTH 5
#define RENDERING_MAP_TILE_HEIGHT 3
#define RENDERING_MAP_TILE_BORDER 0

// color definitions
#define RENDERING_COLOR_PAIR_WHITE 1
#define RENDERING_COLOR_PAIR_RED 2
#define RENDERING_COLOR_PAIR_GREEN 3
#define RENDERING_COLOR_WHITE COLOR_PAIR(RENDERING_COLOR_PAIR_WHITE)
#define RENDERING_COLOR_RED COLOR_PAIR(RENDERING_COLOR_PAIR_RED)
#define RENDERING_COLOR_GREEN COLOR_PAIR(RENDERING_COLOR_PAIR_GREEN)

// tile sprites
char tile_wall[RENDERING_MAP_TILE_HEIGHT][RENDERING_MAP_TILE_WIDTH] =
{
	{ '#', '#', '#', '#', '#' },
	{ '#', '#', '#', '#', '#' },
	{ '#', '#', '#', '#', '#' }
};
char tile_destructive_wall[RENDERING_MAP_TILE_HEIGHT][RENDERING_MAP_TILE_WIDTH] =
{
	{ '#', '#', '#', '#', '#' },
	{ '#', ' ', ' ', ' ', '#' },
	{ '#', '#', '#', '#', '#' }
};
char tile_floor[RENDERING_MAP_TILE_HEIGHT][RENDERING_MAP_TILE_WIDTH] =
{
	{ '.', ' ', '.', ' ', '.' },
	{ ' ', '.', ' ', '.', ' ' },
	{ '.', ' ', '.', ' ', '.' }
};

typedef struct tile_t
{
	char tile_type;
} tile_t;

tile_t map[MAP_WIDTH][MAP_HEIGHT];

void map_initialize(void)
{
	unsigned int x = 0;
	unsigned int y = 0;
	
	for(x = 0; x < MAP_WIDTH; x++)
	{
		for(y = 0; y < MAP_HEIGHT; y++)
		{
			map[x][y].tile_type = rand() % 3 + 1;
		}
	}
}

void map_render_tile(unsigned int tile_x, unsigned int tile_y)
{
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int x_temp = 0;
	unsigned int y_temp = 0;
	
	// enable color
	switch(map[tile_x][tile_y].tile_type)
	{
		case TILE_TYPE_WALL:
			attron(RENDERING_COLOR_RED);
			break;
		case TILE_TYPE_DESTRUCTIVE_WALL:
			attron(RENDERING_COLOR_GREEN);
			break;
		case TILE_TYPE_FLOOR:
			attron(RENDERING_COLOR_WHITE);
			break;
	}
	
	// render sprite at the correct location:
	// loop through the sprite "pixels" and calculate the corresponding position
	// on the screen
	for(y = 0; y < RENDERING_MAP_TILE_HEIGHT; y++)
	{
		for(x = 0; x < RENDERING_MAP_TILE_WIDTH; x++)
		{
			// calculate position of character
			x_temp = RENDERING_MAP_OFFSET_X + (tile_x * (RENDERING_MAP_TILE_WIDTH + RENDERING_MAP_TILE_BORDER)) + x;
			y_temp = RENDERING_MAP_OFFSET_Y + (tile_y * (RENDERING_MAP_TILE_HEIGHT + RENDERING_MAP_TILE_BORDER)) + y;
			
			// output the character at the current position of the sprite
			switch(map[tile_x][tile_y].tile_type)
			{
				case TILE_TYPE_WALL:
					mvaddch(y_temp, x_temp, tile_wall[y][x]);
					break;
				case TILE_TYPE_DESTRUCTIVE_WALL:
					mvaddch(y_temp, x_temp, tile_destructive_wall[y][x]);
					break;
				case TILE_TYPE_FLOOR:
					mvaddch(y_temp, x_temp, tile_floor[y][x]);
					break;
			}
		}
	}
	
	// disable color
	switch(map[tile_x][tile_y].tile_type)
	{
		case TILE_TYPE_WALL:
			attroff(RENDERING_COLOR_RED);
			break;
		case TILE_TYPE_DESTRUCTIVE_WALL:
			attroff(RENDERING_COLOR_GREEN);
			break;
		case TILE_TYPE_FLOOR:
			attroff(RENDERING_COLOR_WHITE);
			break;
	}
}

void map_render(void)
{
	unsigned int x = 0;
	unsigned int y = 0;
	
	for(x = 0; x < MAP_WIDTH; x++)
	{
		for(y = 0; y < MAP_HEIGHT; y++)
		{
			map_render_tile(x, y);
		}
	}
	
	move(0, 0);
}

int main(void)
{
	srand(time(NULL));
	
	map_initialize();
	
	initscr();
	start_color();
	use_default_colors();
	init_pair(RENDERING_COLOR_PAIR_RED, COLOR_RED, -1);
	init_pair(RENDERING_COLOR_PAIR_GREEN, COLOR_GREEN, -1);
	init_pair(RENDERING_COLOR_PAIR_WHITE, COLOR_WHITE, -1);
	map_render();
	refresh();
	getch();
	endwin();
	
	return 0;
}
