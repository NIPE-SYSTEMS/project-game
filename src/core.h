#ifndef __CORE_H__
#define __CORE_H__

#include <stdarg.h>
#include <stdio.h>

#define CORE_FRAME_TIME 100000 // -> 10 fps, specified in microseconds (usleep)

void core_init(void);
void core_main(void);
void core_cleanup(void);

#ifdef DEBUG
void core_init_files(void);
void core_cleanup_files(void);
void core_log(FILE *file, char *fmt, va_list args);
void core_debug(char *fmt, ...);
void core_error(char *fmt, ...);
#else
#define core_debug(...) (void)0
#define core_error(...) (void)0
#endif /* DEBUG */

#endif /* __CORE_H__ */