#ifndef __GAMEPLAY_H__
#define __GAMEPLAY_H__

#define GAMEPLAY_FIELD_HEIGHT 9
#define GAMEPLAY_FIELD_WIDTH 9

#define GAMEPLAY_FIELD(field, x, y) (field[(y) * GAMEPLAY_FIELD_WIDTH + (x)])

#define GAMEPLAY_MOVE_COOLDOWN 5
#define GAMEPLAY_FIRE_TIME 10

#define GAMEPLAY_W_PRESSED (gameplay_keys.key_w == 1)
#define GAMEPLAY_A_PRESSED (gameplay_keys.key_a == 1)
#define GAMEPLAY_S_PRESSED (gameplay_keys.key_s == 1)
#define GAMEPLAY_D_PRESSED (gameplay_keys.key_d == 1)
#define GAMEPLAY_SPACE_PRESSED (gameplay_keys.key_space == 1)
#define GAMEPLAY_F_PRESSED (gameplay_keys.key_f == 1)

typedef enum item_type_t
{
	EMPTY = 0,
	HEALTH = 8,
	EXTRA_BOMB = 6,
	SPEED = 9,
	SHIELD = 11,
} item_type_t;

typedef enum gameplay_players_type_e
{
	GAMEPLAY_PLAYERS_TYPE_USER,
	GAMEPLAY_PLAYERS_TYPE_AI
} gameplay_players_type_t;

typedef struct gameplay_players_player_s
{
	int health_points;
	int movement_cooldown;
	int movement_cooldown_initial;
	int position_y;
	int position_x;
	int placeable_bombs;
	int placed_bombs;
	item_type_t item;
	int item_usage_time;
	int damage_cooldown;
	gameplay_players_type_t type;
	struct gameplay_players_player_s *next;
} gameplay_players_player_t;

typedef enum field_type_t
{
	WALL = 0,
	DESTRUCTIVE = 1,
	FLOOR = 2,
} field_type_t;

typedef struct gameplay_field_s
{
	field_type_t type;
	gameplay_players_player_t *player;
	int explosion;
	int bomb;
	item_type_t item;
	int timing;
} gameplay_field_t;

void gameplay_field_init(void);
void gameplay_players_initialize(void);
// void gameplay_add_player(gameplay_players_player_t *player, int position_x, int position_y);
// void gameplay_remove_player(gameplay_players_player_t *player, int position_x, int position_y);
int gameplay_get_walkable(int position_x, int position_y);
int gameplay_get_bomb_placed(int position_x, int position_y);
void gameplay_place_bomb(int position_x, int position_y);
item_type_t gameplay_get_item(int position_x, int position_y);
void gameplay_key(char gameplay_pressed_key);
void gameplay_buffer(void);
void gameplay_explosion(int x, int y);
gameplay_field_t *gameplay_get_field(void);

#endif /* __GAMEPLAY_H__ */
