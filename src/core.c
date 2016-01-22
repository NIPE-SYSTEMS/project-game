#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>


#include "core.h"
#include "random-drop.h"
#include "graphics.h"
#include "gameplay.h"

#ifdef DEBUG
static void core_init_files(void);
static void core_cleanup_files(void);
static void core_log(FILE *file, char *fmt, va_list args);
#endif /* DEBUG*/

static char core_running = 0;
static FILE *core_file_debug = NULL;
static FILE *core_file_error = NULL;

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
	
	random_drop_init();
}

/**
 * This function starts the main game loop. This function runs periodically
 * all neccessary functions for rendering.
 */
void core_main(void)
{
	core_running = 1;
	int character = 0;
	gameplay_field_init();
	
	while(core_running == 1)
	{
		clear();
		
		gameplay_update();
		
		while((character = getch()) != ERR)
		{
			switch(character)
			{
				case 'q': case 27: // <Q> or <Esc>
					core_debug("Invoked quit event.");
					core_running = 0;
					break;
				case 'w': case 'a': case 's': case 'd': case ' ': case 'f':
					gameplay_key(character);
					break;
				default:
					core_debug("Pressed unhandled key. Keycode: %i", character);
			}
		}
		
		graphics_main();
		
		if(core_running == 1)
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
