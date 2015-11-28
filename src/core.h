#ifndef __CORE_H__
#define __CORE_H__

#define CORE_VSYNC_BUSY_WAITING 0.003

int core_initialize(void);
void core_cleanup(void);
void core_surface_clear(void);
void core_main(void);

#endif /* __CORE_H__ */