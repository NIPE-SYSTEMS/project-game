#ifndef __GAMEPLAY_ITEMS_H__
#define __GAMEPLAY_ITEMS_H__

#define GAMEPLAY_ITEMS_ITEM_DESPAWN 100

typedef enum gameplay_items_item_type_e
{
	EMPTY = 0,
	HEALTH = 8,
	EXTRA_BOMB = 6,
	FIRE = 7,
	SPEED = 9,
	SHIELD = 11,
} gameplay_items_item_type_t;

typedef struct gameplay_items_item_s
{
	gameplay_items_item_type_t type;
	int position_x;
	int position_y;
	int despawn_timer;
	struct gameplay_items_item_s *next;
} gameplay_items_item_t;

void gameplay_items_add_item(gameplay_items_item_type_t type, int position_x, int position_y);
void gameplay_items_cleanup(void);
void gameplay_items_remove(int position_x, int position_y);
void gameplay_items_item_update();
int gameplay_items_item_placed(int position_x, int position_y);
gameplay_items_item_type_t gameplay_items_get_item_type (int position_x, int position_y);
int gameplay_items_test_remove(int position_x, int position_y);

#endif /* __GAMEPLAY_ITEMS_H__ */
