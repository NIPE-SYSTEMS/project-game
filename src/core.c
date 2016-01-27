#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#include "core.h"
#include "random-drop.h"
#include "graphics.h"
#include "gameplay.h"
#include "graphics-sprites.h"
#include "ai-core.h"

#ifdef DEBUG
static void core_init_files(void);
static void core_cleanup_files(void);
static void core_log(FILE *file, char *fmt, va_list args);
static FILE *core_file_debug = NULL;
static FILE *core_file_error = NULL;
#endif /* DEBUG */

static core_state_t core_state = CORE_START_SCREEN;

/**
 * This function initializes everything. It initializes ncurses.
 */
void core_init(void)
{
#ifdef DEBUG
	core_init_files();
#endif /* DEBUG */
	
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	start_color();
	use_default_colors();
	curs_set(0);
	clear();
	
	random_drop_init();
	graphics_sprites_init();
}

/**
 * This function starts the main game loop. This function runs periodically
 * all neccessary functions for rendering.
 */
void core_main(void)
{
	int character = 0;
	gameplay_players_player_t *player = NULL;
	
	while(core_state != CORE_SHUTDOWN)
	{
		clear();
		
		if(core_state == CORE_RUNNING)
		{
			gameplay_update();
		}
		
		while((character = getch()) != ERR)
		{
			switch(character)
			{
				case 'q': case 27: // <Q> or <Esc>
				{
					core_debug("Invoked quit event.");
					core_state = CORE_SHUTDOWN;
					break;
				}
				case 'c':
				{
					core_debug("Showing QR Code");
					core_state = CORE_QR_CODE;
					
					break;
				}
				case 'p':
				{
					if(core_state == CORE_RUNNING)
					{
						core_state = CORE_PAUSED;
						
						core_debug("");
						core_debug(" +----------------------+");
						core_debug(" | TURBO BOMBER PAUSED  |");
						core_debug(" +----------------------+");
						core_debug("");
						core_debug("Press <P> to resume.");
						
						break;
					}
					
					if(core_state == CORE_PAUSED)
					{
						core_state = CORE_RUNNING;
						
						core_debug("");
						core_debug(" +----------------------+");
						core_debug(" | TURBO BOMBER RESUMED |");
						core_debug(" +----------------------+");
						core_debug("");
						
						break;
					}
				}
				case 'w': case 'a': case 's': case 'd': case ' ': case 'f': case 't': case 'u': case 'r': case 'b': case 'o':
				{
					// skip start screen
					if(character == ' ' && core_state == CORE_START_SCREEN)
					{
						core_state = CORE_MENU;
						break;
					}
					
					// start game
					if(character == ' ' && core_state == CORE_MENU)
					{
						core_state = CORE_RUNNING;
						gameplay_init();
						break;
					}
					
					// return to menu
					if(character == ' ' && (core_state == CORE_WIN || core_state == CORE_GAME_OVER))
					{
						core_state = CORE_MENU;
						break;
					}
					
					// only process keyboard input when game is unpaused
					if(core_state != CORE_PAUSED)
					{
						gameplay_key(character);
					}
					
					break;
				}
				default:
				{
					core_debug("Pressed unhandled key. Keycode: %i", character);
					break;
				}
			}
		}
		
		switch(core_state)
		{
			case CORE_START_SCREEN:
			{
				if(graphics_startscreen() == 0)
				{
					core_state = CORE_MENU;
				}
				
				break;
			}
			case CORE_MENU:
			{
				graphics_render_menu();
				
				break;
			}
			case CORE_RUNNING: case CORE_PAUSED:
			{
				graphics_render_field();
				graphics_render_players();
#ifdef DEBUG_INFO
				graphics_render_debug();
#else
				graphics_render_information();
#endif /* DEBUG_INFO */
				
				player = gameplay_players_get_user();
				if(player->health_points == 0)
				{
					core_state = CORE_GAME_OVER;
					gameplay_cleanup();
				}
				
				break;
			}
			case CORE_QR_CODE:
			{
				graphics_render_qr_code();
				
				break;
			}
			case CORE_WIN:
			{
				core_state = CORE_GAME_OVER;
				
				break;
			}
			case CORE_GAME_OVER:
			{
				graphics_game_over_function();
				
				break;
			}
			default:
			{
				break;
			}
		}
		
		move(0, 0);
		refresh();
		
		if(core_state != CORE_SHUTDOWN)
		{
			usleep(CORE_FRAME_TIME);
		}
	}
}

/**
 * This function cleans everything up.
 */
void core_cleanup(void)
{
	graphics_sprites_cleanup();
	gameplay_cleanup();
	
	endwin();
	
#ifdef DEBUG
	core_cleanup_files();
#endif /* DEBUG */
}

#ifdef DEBUG

/**
 * This function initializes output files.
 */
static void core_init_files(void)
{
	core_file_debug = fopen("debug.log", "a");
	if(core_file_debug == NULL)
	{
		fprintf(stderr, "Failed to open debug log fvoid core_init(void)ile.\n");
		exit(1);
		return;
	}
	
	core_file_error = fopen("error.log", "a");
	if(core_file_error == NULL)
	{
		fprintf(stderr, "Failed to open error log file.\n");
		fclose(core_file_debug);
		exit(1);
		return;
	}
	
	core_debug("");
	core_debug(" +----------------------+");
	core_debug(" | TURBO BOMBER STARTED |");
	core_debug(" +----------------------+");
	core_debug("");
	core_debug("Log system initialized.");
}

/**
 * This function cleans up output files.
 */
static void core_cleanup_files(void)
{
	core_debug("");
	core_debug(" +----------------------+");
	core_debug(" | TURBO BOMBER STOPPED |");
	core_debug(" +----------------------+");
	core_debug("");
	
	if(core_file_debug != NULL)
	{
		fclose(core_file_debug);
	}
	
	if(core_file_error != NULL)
	{
		fclose(core_file_error);
	}
}

/**
 * This function generates a log message and writes it to a file pointer.
 */
static void core_log(FILE *file, char *fmt, va_list args)
{
	char time_buffer[80];
	time_t rawtime;
	struct tm * timeinfo;
	
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	
	strftime(time_buffer, 80, "%c: ", timeinfo);
	
	fwrite(time_buffer, strlen(time_buffer), 1, file);
	
	vfprintf(file, fmt, args);
	
	fwrite("\n", strlen("\n"), 1, file);
	
	fflush(file);
}

/**
 * This function outputs a debug message to the debug logging file.
 * 
 * @param fmt The format string like in "printf".
 * @param ... The arguments for the format like in "printf".
 */
void core_debug(char *fmt, ...)
{
	va_list args;
	
	va_start(args, fmt);
	core_log(core_file_debug, fmt, args);
	va_end(args);
}

/**
 * This function outputs an error message to the error logging file.
 * 
 * @param fmt The format string like in "printf".
 * @param ... The arguments for the format like in "printf".
 */
void core_error(char *fmt, ...)
{
	va_list args;
	
	va_start(args, fmt);
	core_log(core_file_error, fmt, args);
	va_end(args);
}

#endif /* DEBUG */
