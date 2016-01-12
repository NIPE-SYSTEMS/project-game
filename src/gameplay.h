#ifndef __GAMEPLAY_H__
#define __GAMEPLAY_H__

#define GAMEPLAY_FIELD_HEIGHT 5
#define GAMEPLAY_FIELD_WIDTH 5

#define GAMEPLAY_MOVE_COOLDOWN 10
#define GAMEPLAY_FIRE_TIME 10

typedef enum field_type_t
{
	WALL,
	DESTRUCTIVE,
	FLOOR
} field_type_t;

typedef enum item_type_t
{
	EMPTY,
	HEALTH,
	EXTRA_BOMB,
	SPEED,
	SHIELD,
} item_type_t;

typedef struct gameplay_field_t
{
	field_type_t type;
	int player;
	int explosion;
	int bomb;
	item_type_t item;
	int timing;
} gameplay_field_t;

typedef struct gameplay_player_t
{
	int health_points;
	int movement_cooldown;
	int position_height;
	int position_width;
	int placeable_bombs;
	int placed_bombs;
	item_type_t item;
	int item_usage_time;
	int damage_cooldown;
} gameplay_player_t;

typedef struct gameplay_keys_t
{
	int key_w;
	int key_a;
	int key_s;
	int key_d;
	int key_space;
	int key_f;
} gameplay_keys_t;

gameplay_field_t gameplay_field[GAMEPLAY_FIELD_HEIGHT][GAMEPLAY_FIELD_WIDTH];
gameplay_player_t gameplay_player;
gameplay_keys_t gameplay_keys;

void gameplay_field_init();
void gameplay_key_reset();
void gameplay_key(char gameplay_pressed_button);
void gameplay_interpret();
void gameplay_test_move_up();
void gameplay_test_move_down();
void gameplay_test_move_left();
void gameplay_test_move_right();
void gameplay_test_place_bomb();
void gameplay_buffer();
void gameplay_test_item_pick_up();
void gameplay_use_item();
void gameplay_explosion(int height, int width);

#endif /* __GAMEPLAY_H__ */