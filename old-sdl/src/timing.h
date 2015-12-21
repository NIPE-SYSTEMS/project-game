#ifndef __TIMER_H__
#define __TIMER_H__

#include <sys/time.h>

typedef void (*timing_interval_callback_t)(void);

double timing_timestamp_get(void);
void timing_interval_callbacks_run(void);

#endif /* __TIMER_H__ */