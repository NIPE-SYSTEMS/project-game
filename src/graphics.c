#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdio.h>

#include "graphics.h"
#include "graphics-sprites.h"
#include "gameplay.h"
#include "gameplay-players.h"
#include "gameplay-bombs.h"
#include "core.h"
#include "gameplay-items.h"
#include "ai-core.h"

static int set_show_debug = 0;
static int graphics_animation_counter = 0;
static int graphics_startscreen_counter = 0;
static int graphics_frames_for_startscreen = 60;
static int graphics_game_over_counter = 0;
static int graphics_game_over_checker = 0;
static struct graphics_sprite_s graphics_list_of_sprites[] =
{
	{GRAPHICS_SPRITES_UNDESTROYABLE, "assets/undestructable.sprite", 5, 3, NULL},
	{GRAPHICS_SPRITES_DESTROYABLE, "assets/destructable.sprite", 5, 3, NULL},
	{GRAPHICS_SPRITES_DESTROYED, "assets/destroyed.sprite", 5, 3, NULL},
	{GRAPHICS_SPRITES_PLAYER, "assets/player.sprite", 5, 3, NULL},
	{GRAPHICS_SPRITES_PLAYER_STANDING, "assets/player_standing.sprite", 5, 3, NULL},
	{GRAPHICS_SPRITES_ENEMY, "assets/enemy.sprite", 5, 3, NULL},
	{GRAPHICS_SPRITES_ENEMY_STANDING, "assets/enemy_standing.sprite", 5, 3, NULL},
	{GRAPHICS_SPRITES_BOMB, "assets/bomb.sprite", 5, 3, NULL},
	{GRAPHICS_SPRITES_BOMB_UP, "assets/bomb_up.sprite", 5, 3, NULL},
	{GRAPHICS_SPRITES_FIRE, "assets/fire.sprite", 5, 3, NULL},
	{GRAPHICS_SPRITES_HEART, "assets/heart.sprite", 5, 3, NULL},
	{GRAPHICS_SPRITES_SNEAKER, "assets/sneaker.sprite", 5, 3, NULL},
	{GRAPHICS_SPRITES_UP, "assets/one_up.sprite", 5, 3, NULL},
	{GRAPHICS_SPRITES_SHIELD, "assets/shield.sprite", 5, 3, NULL},
	{GRAPHICS_SPRITES_EXPLOSION_1, "assets/explosion_1.sprite", 5, 3, NULL},
	{GRAPHICS_SPRITES_EXPLOSION_2, "assets/explosion_2.sprite", 5, 3, NULL}
};

void graphics_show_debug(void)
{
	int i = 0;
	int offset_line = 0;
	int player_amount = 0;
	gameplay_players_player_t *player = NULL;
	int bomb_amount = 0;
	gameplay_bombs_bomb_t *bomb = NULL;
	
	mvprintw(GRAPHICS_DEBUG_Y + offset_line++, GRAPHICS_DEBUG_X, "Players [");
	player_amount = gameplay_players_amount();
	for(i = 0; i < player_amount; i++)
	{
		player = gameplay_players_get(i);
		if(player == NULL)
		{
			continue;
		}
		
		mvprintw(GRAPHICS_DEBUG_Y + offset_line++, GRAPHICS_DEBUG_X + 2, "%s { h: %i, m: %i (%i), p: (%i, %i), b: %i (%i), e: %i, d: %i (%i) }", ((player->type == GAMEPLAY_PLAYERS_TYPE_AI)?("AI  "):("USER")), player->health_points, player->movement_cooldown, player->movement_cooldown_initial, player->position_x, player->position_y, player->placed_bombs, player->placeable_bombs, player->explosion_radius, player->damage_cooldown, player->damage_cooldown_initial);
	}
	mvprintw(GRAPHICS_DEBUG_Y + offset_line++, GRAPHICS_DEBUG_X, "]");
	
	offset_line++;
	
	mvprintw(GRAPHICS_DEBUG_Y + offset_line++, GRAPHICS_DEBUG_X, "Bombs [");
	bomb_amount = gameplay_bombs_amount();
	for(i = 0; i < bomb_amount; i++)
	{
		bomb = gameplay_bombs_get(i);
		if(bomb == NULL)
		{
			continue;
		}
		
		mvprintw(GRAPHICS_DEBUG_Y + offset_line++, GRAPHICS_DEBUG_X + 2, "{ o: %s, p: (%i, %i), e: %i }", ((bomb->owner->type == GAMEPLAY_PLAYERS_TYPE_AI)?("AI"):("USER")), bomb->position_x, bomb->position_y, bomb->explosion_timeout);
	}
	mvprintw(GRAPHICS_DEBUG_Y + offset_line++, GRAPHICS_DEBUG_X, "]");
}

/**
 * This function renders a sprite at a given position. It also can handle
 * transparency.
 * 
 * @param pos_x The X position.
 * @param pos_y The Y position.
 * @param index The index of the sprite.
 * @param transparency 1 to enable transparency rendering, 0 to disable it. 0
 *                     means that every new rendering will overwrite the
 *                     existing characters.
 */
void graphics_render_sprite(int pos_x, int pos_y, graphics_sprites_t index, char transparency)
{
	int x = 0;
	int y = 0;
	
	init_pair(1, COLOR_WHITE, -1);//UNDESTROYABLE
	init_pair(2, COLOR_WHITE, -1);//DESTROYABLE
	init_pair(3, COLOR_RED, -1);//DESTROYED
	init_pair(4, COLOR_GREEN, -1);//PLAYER
	init_pair(5, COLOR_GREEN, -1);//PLAYER_STANDING
	init_pair(6, COLOR_RED, -1);//ENEMY
	init_pair(7, COLOR_RED, -1);//ENEMY_STANDING
	init_pair(8, COLOR_RED, -1);//BOMB
	init_pair(9, COLOR_YELLOW, -1);//BOMB_UP
	init_pair(10, COLOR_YELLOW, -1);//FIRE
	init_pair(11, COLOR_RED, -1);//HEART
	init_pair(12, COLOR_YELLOW, -1);//SNEAKERS
	init_pair(13, COLOR_GREEN, -1);//1_UP
	init_pair(14, COLOR_YELLOW, -1);//SHIELD
	init_pair(15, COLOR_YELLOW, -1);//EXPLOSION
	init_pair(16, COLOR_RED, -1);//EXPLOSION
	init_pair(17, COLOR_GREEN, -1);//PLACABLE BOMBS
	init_pair(18, COLOR_BLUE, -1);//SPEED
	init_pair(19, COLOR_BLACK, -1);//Normal
	
	//TURBO MODE
	init_pair(20, COLOR_WHITE, -1);
	init_pair(21, COLOR_RED, -1);
	init_pair(22, COLOR_BLUE, -1);
	init_pair(23, COLOR_GREEN, -1);
	init_pair(24, COLOR_BLACK, -1);
	init_pair(25, COLOR_YELLOW, -1);
	init_pair(26, COLOR_CYAN, -1);
	init_pair(27, COLOR_MAGENTA, -1);
	
	for(y = 0; y < GRAPHICS_SPRITE_HEIGHT; y++)
	{
		for(x = 0; x < GRAPHICS_SPRITE_WIDTH; x++)
		{
			// if transparency == 1 then render only if character of sprite != ' '
			if(transparency == 0 || (transparency == 1 && graphics_sprites[index][y][x] != ' '))
			{
				if((animation_turbo_activated == 1 && index == 3) || (animation_turbo_activated == 1 && index == 4))
				{
					attron(COLOR_PAIR((rand()%8)+20));
					mvaddch(pos_y + y, pos_x + x, graphics_sprites[index][y][x]);
					attron(COLOR_PAIR(19));
				}
				else
				{
					attron(COLOR_PAIR(index+1));
					mvaddch(pos_y + y, pos_x + x, graphics_sprites[index][y][x]);
					attron(COLOR_PAIR(19));
				}
			}
		}
	}
}

int graphics_text_to_array(int iterator, int width, int height)
{
	//int i = 0;
	char *array;
	array = malloc(width*height*sizeof(char));
	
	FILE *current_file;
	//current_file = fopen("text.sprite", "r");
	current_file = fopen(graphics_list_of_sprites[iterator].path, "r");
	/*
	if (current_file == NULL)
	{
		printf("error while opening file\n"); //ausgabe muss noch verändert werden!!!
		return 0;
	}
	*/
	//i = 0;
	 mvprintw(0, 48, "Debug Test!");//debug
	/*
	while(i < (width*height))
	{
		array[i] = getc(current_file);
		if(array[i] == ' ' || array[i] == '#' || array[i] == 'o' ||array[i] == '\\' || array[i] == '|' || array[i] == '/' || array[i] == 'Y' || array[i] == 'L' || array[i] == 'D' || array[i] == '_' || array[i] == '-' || array[i] == '+' || array[i] == '~' || array[i] == '1' || array[i] == 'V' || array[i] == 'v' || array[i] == 'O' || array[i] == 'T')
		{
			i++;
		}
	}
	*/
	fclose(current_file);
	graphics_list_of_sprites[iterator].data = array;
	return 0;
}

void graphics_read_arrays(void)
{
	int sprite_amount = 16;
	int static_sprite_width = 5;
	int static_sprite_height = 3;
	int i;
	
	for (i = 0; i < sprite_amount; i++)
	{
		graphics_text_to_array(i, static_sprite_width, static_sprite_height);
	}
}
/*
char *graphics_read_array(char *path, int width, int height)
{
	char *array = NULL;
	array = malloc (width*height*sizeof(char));
	int i = 0; //Debug
	
	FILE *current_file;
	current_file = fopen( "./assets/undestructable.sprite" *path, "r");
	{
			fprintf(stderr, "Failed to open file.\n");
			return 0;
	}
	
	//DEBUG
	for (i = 0; i < (width*height); i++)
	{
		fprintf(stderr, "%c", array++);
	}
	
	//
	return *array;
}

void graphics_get_arrays (void)
{
	int i = 0;
	int e = 0;
	char c = 'q'; //nur übergangsweise
	
	for( i= 0; i < 16; i++)
	{
		FILE *current_file;
		current_file = fopen(graphics_list_of_sprites[i].path, "r");
		
		if(current_file == NULL)
		{
			fprintf(stderr, "Failed to open file.\n");
			return;
		}
		e = 0;
		for()
		{
		
			c = getc (current_file);
			e++;
		}
	}
}
*/

	/*
	FILE *file_game_over;
	char *Game_Over_Frames = NULL;
	
	int c;
	int counter = 0;
	file_game_over = fopen ("Game_Over_Array.txt", "r");
	
	if(file_game_over == NULL)
	{
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	do
	{
		c = getc (file_game_over);
		counter++;
	}while (c != EOF);
	
	
	Game_Over_Frames = malloc(counter * sizeof(char));
	
	do
	{
		Game_Over_Frames = getc (file_game_over);
		*Game_Over_Frames++;
	}while (c != EOF);
	
	fclose (file_game_over);
	
}

*/

/**
 * This function will render the startscreen which will
 * display the games logo and name.
 */
void graphics_startscreen(void)
{
	init_pair(1, COLOR_WHITE, -1);
	init_pair(2, COLOR_RED, -1);
	init_pair(3, COLOR_BLUE, -1);
	init_pair(4, COLOR_GREEN, -1);
	init_pair(5, COLOR_YELLOW, -1);
	init_pair(6, COLOR_CYAN, -1);
	init_pair(7, COLOR_MAGENTA, -1);
	
	int i = 0;
	int b = 0;
	
	if (animation_turbo_activated == 1)
	{
		for(i = 0; i < 26; i++)
		{
			for(b = 0; b < 103; b++)
			{
				attron(COLOR_PAIR(rand()%8));
				mvaddch(3+i, 3+b, displayed_text[i][b]);
				attron(COLOR_PAIR(1));
			}
		}
	}
	else
	{
		for(i = 0; i < 26; i++)
		{
			for(b = 0; b < 103; b++)
			{
				attron(COLOR_PAIR(1));
				mvaddch(3+i, 3+b, displayed_text[i][b]);
			}
		}
	}
	
	if(graphics_startscreen_counter <= 30)
	{
		for(i = 0; i < 26; i++)
		{
			for(b = 0; b < 103; b++)
			{
				if(startscreen_frames[graphics_startscreen_counter][i][b] != '#')
				{
					//attron(COLOR_PAIR(2));
					mvaddch(3+i, 3+b, startscreen_frames[graphics_startscreen_counter][i][b]);
					//attron(COLOR_PAIR(1));
				}
			}
		}
	}
	move(0, 0);
	refresh();
}

void graphics_game_over_function(void)
{
	init_pair(1, COLOR_WHITE, -1);
	init_pair(2, COLOR_RED, -1);
	init_pair(3, COLOR_BLUE, -1);
	init_pair(4, COLOR_GREEN, -1);
	init_pair(5, COLOR_YELLOW, -1);
	init_pair(6, COLOR_CYAN, -1);
	init_pair(7, COLOR_MAGENTA, -1);
	//init_pair(8, COLOR_BLACK, -1);
	
	int i = 0;
	int b = 0;
	
	for(i = 0; i < 35; i++)
	{
		for(b = 0; b < 69; b++)
		{
			if(graphics_game_over_counter <= 13)
			{
				attron(COLOR_PAIR(1));
				//attron(COLOR_PAIR(rand()%9));
				mvaddch(3+i, 3+b, graphics_game_over[graphics_game_over_counter][i][b]);
				//attron(COLOR_PAIR(1));
			}
			else
			{
				if(animation_turbo_activated == 1)
				{
					if(graphics_game_over[13][i][b] == '#')
					{
						attron(COLOR_PAIR(rand()%8));
						mvaddch(3+i, 3+b, graphics_game_over[13][i][b]);
					}
				}
				else
				{
					if(graphics_game_over[13][i][b] == '#')
					{
						attron(COLOR_PAIR(2));
						mvaddch(3+i, 3+b, graphics_game_over[13][i][b]);
					}
				}
			}
			
		}
	}
	graphics_game_over_counter++;
	/*
	if(graphics_game_over_counter < 13)
	{
		graphics_game_over_counter++;
	}
	*/
	
	move(0, 0);
	refresh();
}

void graphics_render_players(void)
{
	gameplay_players_player_t *player = NULL;
	int player_amount = 0;
	int render_x = 0;
	int render_y = 0;
	int i = 0;
	
	player_amount = gameplay_players_amount();
	for(i = 0; i < player_amount; i++)
	{
		player = gameplay_players_get(i);
		if(player == NULL)
		{
			continue;
		}
		if(player->damage_cooldown > 0 && graphics_animation_counter%2 == 0)
		{
			render_x = (player->position_x * GRAPHICS_OFFSET_X) + GRAPHICS_OFFSET_X - GRAPHICS_SPRITE_WIDTH;
			render_y = (player->position_y * GRAPHICS_OFFSET_Y) + GRAPHICS_OFFSET_Y - GRAPHICS_SPRITE_HEIGHT;
			
			if(player->movement_cooldown > 1)
			{
				if(player->type == GAMEPLAY_PLAYERS_TYPE_USER)
				{
					graphics_render_sprite(render_x, render_y, GRAPHICS_SPRITES_PLAYER, 1);
				}
				else
				{
					graphics_render_sprite(render_x, render_y, GRAPHICS_SPRITES_ENEMY, 1);
				}
				
				}
			else
			{
				if(player->type == GAMEPLAY_PLAYERS_TYPE_USER)
				{
					graphics_render_sprite(render_x, render_y, GRAPHICS_SPRITES_PLAYER_STANDING, 1);
				}
				else
				{
					graphics_render_sprite(render_x, render_y, GRAPHICS_SPRITES_ENEMY_STANDING, 1);
				}
			}
		}
		else if (player->damage_cooldown == 0)
		{
			render_x = (player->position_x * GRAPHICS_OFFSET_X) + GRAPHICS_OFFSET_X - GRAPHICS_SPRITE_WIDTH;
			render_y = (player->position_y * GRAPHICS_OFFSET_Y) + GRAPHICS_OFFSET_Y - GRAPHICS_SPRITE_HEIGHT;
			
			if(player->movement_cooldown > 1)
			{
				if(player->type == GAMEPLAY_PLAYERS_TYPE_USER)
				{
					graphics_render_sprite(render_x, render_y, GRAPHICS_SPRITES_PLAYER, 1);
				}
				else
				{
					graphics_render_sprite(render_x, render_y, GRAPHICS_SPRITES_ENEMY, 1);
				}
				
				}
			else
			{
				if(player->type == GAMEPLAY_PLAYERS_TYPE_USER)
				{
					graphics_render_sprite(render_x, render_y, GRAPHICS_SPRITES_PLAYER_STANDING, 1);
				}
				else
				{
					graphics_render_sprite(render_x, render_y, GRAPHICS_SPRITES_ENEMY_STANDING, 1);
				}
			}
		}
	}
}

void graphics_render_information(void)
{
	init_pair(1, COLOR_WHITE, -1);
	
	int i = 0;
	gameplay_players_player_t *player = NULL;
	
	player = gameplay_players_get_user();
	
	// hearts
	attron(COLOR_PAIR(1));
	mvprintw(GRAPHICS_HEALTH_Y, GRAPHICS_HEALTH_X, "Health:");
	player = gameplay_players_get_user();
	
	for(i = 0; i < player->health_points; i++)
	{
		graphics_render_sprite(GRAPHICS_HEALTH_X + (GRAPHICS_HEALTH_OFFSET_X * i), GRAPHICS_HEALTH_Y + GRAPHICS_HEALTH_OFFSET_Y, GRAPHICS_SPRITES_HEART, 0);
	}
	
	// bombs
	attron(COLOR_PAIR(1));
	mvprintw(GRAPHICS_HEALTH_Y+7, GRAPHICS_HEALTH_X, "Placed Bombs:");
	player = gameplay_players_get_user();
	
	for(i = 0; i < player->placeable_bombs; i++)
	{
		graphics_render_sprite(GRAPHICS_HEALTH_X + (GRAPHICS_HEALTH_OFFSET_X * i), GRAPHICS_HEALTH_Y + GRAPHICS_HEALTH_OFFSET_Y+7, GRAPHICS_SPRITES_BOMBS_PLACABLE, 0);
	}
	
	for(i = 0; i < player->placed_bombs; i++)
	{
		graphics_render_sprite(GRAPHICS_HEALTH_X + (GRAPHICS_HEALTH_OFFSET_X * i), GRAPHICS_HEALTH_Y + GRAPHICS_HEALTH_OFFSET_Y+7, GRAPHICS_SPRITES_BOMB, 0);
	}
	
	attron(COLOR_PAIR(1));
	mvprintw(GRAPHICS_HEALTH_Y+14, GRAPHICS_HEALTH_X, "Speed:");
	player = gameplay_players_get_user();
	
	for(i = 0; i < (6-player->movement_cooldown_initial); i++)
	{
		graphics_render_sprite(GRAPHICS_HEALTH_X + (GRAPHICS_HEALTH_OFFSET_X * i), GRAPHICS_HEALTH_Y + GRAPHICS_HEALTH_OFFSET_Y+14, GRAPHICS_SPRITES_SPEED, 0);
	}
	
	attron(COLOR_PAIR(1));
	mvprintw(GRAPHICS_HEALTH_Y+21, GRAPHICS_HEALTH_X, "Blast radius:");
	player = gameplay_players_get_user();
	
	for(i = 0; i < player->explosion_radius; i++)
	{
		graphics_render_sprite(GRAPHICS_HEALTH_X + (GRAPHICS_HEALTH_OFFSET_X * i), GRAPHICS_HEALTH_Y + GRAPHICS_HEALTH_OFFSET_Y+21, GRAPHICS_SPRITES_EXPLOSION_1, 0);
	}
	
	/*
	mvprintw(20, 77, "Current item:");
	if(gameplay_player->item == 0)
	{
	graphics_render_sprite(22,82, 2, 0);
	}
	else
	{
	graphics_render_sprite(22,82, gameplay_player->item, 0);
	}
	*/
}

void graphics_render_field(void)
{
	int x = 0;
	int y = 0;
	int field_index = 0;
	int render_x = 0;
	int render_y = 0;
	gameplay_field_t *gameplay_field = NULL;
	
	gameplay_field = gameplay_get_field();
	
	for(y = 0; y < GAMEPLAY_FIELD_HEIGHT; y++)
	{
		for(x = 0; x < GAMEPLAY_FIELD_WIDTH; x++)
		{
			field_index = y * GAMEPLAY_FIELD_WIDTH + x;
			render_x = (x * GRAPHICS_OFFSET_X) + GRAPHICS_OFFSET_X - GRAPHICS_SPRITE_WIDTH;
			render_y = (y * GRAPHICS_OFFSET_Y) + GRAPHICS_OFFSET_Y - GRAPHICS_SPRITE_HEIGHT;
			
			if(gameplay_bombs_get_bomb_placed(x, y) == 1) // bomb
			{
				graphics_render_sprite(render_x, render_y, GRAPHICS_SPRITES_BOMB, 0);
			}
			else if(gameplay_get_fire(x, y) == 1) // fire
			{
				attron(COLOR_PAIR(2));
				graphics_render_sprite(render_x, render_y, GRAPHICS_SPRITES_EXPLOSION_1 + graphics_animation_counter % 2, 0);
				attron(COLOR_PAIR(1));
			}
			else if(gameplay_items_item_placed(x, y) != 0) // item
			{
				graphics_render_sprite(render_x, render_y, gameplay_items_get_item_type(x, y), 0);
			}
			else // field
			{
				graphics_render_sprite(render_x, render_y, gameplay_field[field_index].type, 0);
			}
		}
	}
}

/**
 * This function handles everything of the rendering. It will render the user
 * interface by interpreting the field of the gameplay module.
 */
void graphics_main(void)
{
	gameplay_players_player_t *player = NULL;
	//graphics_get_arrays ();
	
	//I believe, that the Ai can already make decisions before the player gets to see the field. That is why the start screen should delay the entire game and not just the rendering.
	if (graphics_startscreen_counter >= graphics_frames_for_startscreen && graphics_game_over_checker == 0)
	{
		if(graphics_startscreen_counter >= (graphics_frames_for_startscreen + 1)) //Delayes the start of the AI by one frame
		{
			ai_core_enable();
		}
		else
		{
			graphics_startscreen_counter++;
		}
		
		init_pair(1, COLOR_RED, -1);
		player = gameplay_players_get_user();
		
		//Game Over scrren activation
		if (player->health_points == 0)
		{
			graphics_game_over_checker = 1;
		}
		
		//Debug Information
		attron(COLOR_PAIR(1));
		
		if (set_show_debug)
		{
			graphics_show_debug();
		}
		else
		{
			graphics_render_information();
		}
		
		graphics_render_field();
		graphics_render_players();
		
		graphics_animation_counter++;
		move(0, 0);
		refresh();
	}
	else if (graphics_animation_counter == 0)
	{
		graphics_startscreen();
		graphics_startscreen_counter++;
	}
	else if(graphics_game_over_checker == 1)
	{
		//hier muss noch AI cleanup dazu!
		graphics_game_over_function();
	}
}

void graphics_render_breaked_game()
{
	attron(COLOR_PAIR(1));
	if (set_show_debug)
	{
		graphics_show_debug();
	}
	else
	{
		graphics_render_information();
	}
	
	graphics_render_field();
	graphics_render_players();
	move(0, 0);
	refresh();
}