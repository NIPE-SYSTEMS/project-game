#ifndef __GAMEPLAY_H__
#define __GAMEPLAY_H__

#define GAMEPLAY_FIELD_HEIGHT 9
#define GAMEPLAY_FIELD_WIDTH 9

#define GAMEPLAY_FIELD(field, x, y) (field[(y) * GAMEPLAY_FIELD_WIDTH + (x)])

#include "gameplay-items.h"
#include "gameplay-players.h"

typedef enum gameplay_field_type_t
{
	WALL = 0,
	DESTRUCTIVE = 1,
	FLOOR = 2,
} gameplay_field_type_t;

typedef struct gameplay_field_s
{
	gameplay_field_type_t type;
	gameplay_items_item_t item;
} gameplay_field_t;

void gameplay_field_init(void);
void gameplay_players_initialize(void);
void gameplay_cleanup(void);
int gameplay_get_walkable(int position_x, int position_y);
gameplay_items_item_t gameplay_get_item(int position_x, int position_y);
void gameplay_destroy(int position_x, int position_y);
void gameplay_key(char gameplay_pressed_key);
void gameplay_update(void);
void gameplay_explosion(int x, int y);
gameplay_field_t *gameplay_get_field(void);

#endif /* __GAMEPLAY_H__ */
