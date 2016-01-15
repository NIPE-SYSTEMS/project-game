#ifndef __GAMEPLAY_H__
#define __GAMEPLAY_H__

#define GAMEPLAY_FIELD_HEIGHT 9
#define GAMEPLAY_FIELD_WIDTH 9

#define GAMEPLAY_MOVE_COOLDOWN 5
#define GAMEPLAY_FIRE_TIME 10


typedef enum field_type_t
{
	WALL = 0,
	DESTRUCTIVE = 1,
	FLOOR = 2,
} field_type_t;

typedef enum item_type_t
{
	EMPTY = 0,
	HEALTH = 8,
	EXTRA_BOMB = 6,
	SPEED = 9,
	SHIELD = 11,
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
	int position_y;
	int position_x;
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

void gameplay_field_init(void);
void gameplay_key_reset(void);
void gameplay_key(char gameplay_pressed_button);
void gameplay_interpret(void);
void gameplay_test_move_up(void);
void gameplay_test_move_down(void);
void gameplay_test_move_left(void);
void gameplay_test_move_right(void);
void gameplay_test_place_bomb(void);
void gameplay_buffer(void);
void gameplay_test_item_pick_up(void);
void gameplay_use_item(void);
void gameplay_explosion(int height, int width);
gameplay_field_t *gameplay_get_field(void);
gameplay_player_t *gameplay_get_player(void);

#endif /* __GAMEPLAY_H__ */