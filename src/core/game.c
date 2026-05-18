#include <game.h>
#include <stdint.h>
#include <stdlib.h>
struct Point apple = {0,0};

// our snake can have a max length of (78*22) - 1 = 1715 

uint32_t game_speed_multiplier = 10;
uint32_t current_tick = 0;

#define MAX_ROWS (23)
#define MAX_COLS (78)
#define MAX_SNAKE_LENGTH (MAX_ROWS * MAX_COLS - 1)
struct Point snake[MAX_SNAKE_LENGTH] = {{0,0},{40, 12}};

struct Point *head = (snake + 1);
struct Point *tail = snake;

enum MOVEMENT last_ticked_movement = MOVE_NONE;

static inline uint8_t apple_spawned() {
	return apple.x != 0 || apple.y != 0;
}

static inline void spawn_apple() {
retry:
	apple.x = (rand() % MAX_COLS) + 1;
	apple.y = (rand() % MAX_ROWS) + 1;

	//make sure we're not touching the snake with the apple	
	struct Point *current = tail;
	while (current != head) {
		if (current->x == apple.x && current->y == apple.y) {
			goto retry;
		}
		current = snake + ((current - snake + 1) % MAX_SNAKE_LENGTH);
	}
	if (head->x == apple.x && head->y == apple.y) {
		goto retry;
	}

}

static inline uint16_t snake_length() {
	if (head >= tail) {
		return head - tail + 1;
	}
	return MAX_SNAKE_LENGTH - (tail - head) + 1;
}

static inline uint8_t is_eating_apple() {
	if (head->x == apple.x && head->y == apple.y) {
		return 1;
	}
	return 0;
}

static inline uint8_t is_out_of_bounds() {
	if (head->x <= 0 || head->x > MAX_COLS || head->y <= 0 || head->y > MAX_ROWS) {
		return 1;
	}
	return 0;
}

static inline uint8_t snake_intersects_self() {
	struct Point *start = snake + ((tail - snake + 1) % MAX_SNAKE_LENGTH);
	while (start != head) {
		if (head->x == start->x && head->y == start->y) {
			return 1;
		}
		start = snake + ((start - snake + 1) % MAX_SNAKE_LENGTH);
	}
	return 0;
}

static inline void update_head() {
	struct Point *new_head = snake + ( (head - snake + 1) % MAX_SNAKE_LENGTH);
	new_head->x = head->x;
	new_head->y = head->y;
	head = new_head;
}

void move_snake(enum MOVEMENT dir) {
	// when we move the snake, simply enqueue a new head, and if we are not growing, dequeue the tail
	update_head();
	last_ticked_movement = current_movement;
	switch (last_ticked_movement) {
	case MOVE_UP:
		head->y -= 1;
		break;
	case MOVE_DOWN:
		head->y += 1;
		break;
	case MOVE_LEFT:
		head->x -= 1;
		break;
	case MOVE_RIGHT:
		head->x += 1;
		break;
	default:
		break;
	}
	if (!is_eating_apple()) {
		tail = snake + ((tail - snake + 1) % MAX_SNAKE_LENGTH);
	}
}

void draw_snake() {
	struct Point *current = head;
	kputs("O", make_color(YELLOW, BLACK, 0), current->x, current->y);
	current = snake + ((current - snake - 1 + MAX_SNAKE_LENGTH) % MAX_SNAKE_LENGTH);
	while (current != tail) {
		kputs("O", make_color(LIGHT_GREEN, BLACK, 0), current->x, current->y);
		current = snake + ((current - snake - 1 + MAX_SNAKE_LENGTH) % MAX_SNAKE_LENGTH);
	}
}
char score_buffer[32];
void update_game_state() {

	if (is_out_of_bounds() || snake_intersects_self()) {
		kputs("loser", make_color(LIGHT_RED, BLACK, 0), 36, 12);
		kputs("score: ", make_color(LIGHT_RED, BLACK, 0), 35, 13);
		itoa((snake_length()-1), score_buffer);
		kputs(score_buffer, make_color(LIGHT_RED, BLACK, 0), 41, 13);
		kputs("press R to restart", make_color(LIGHT_RED, BLACK, 0), 30, 14);
		return;
	}
	
	if (snake_length() == MAX_SNAKE_LENGTH) {
		kputs("winer", make_color(LIGHT_GREEN, BLACK, 0), 36, 12);
		kputs("press R to restart", make_color(LIGHT_GREEN, BLACK, 0), 30, 14);
		return;
	}
	
	uint32_t sync_ticks = get_ticks()*game_speed_multiplier / TARGET_FREQ;
	if (sync_ticks > current_tick && !is_paused()) {
		current_tick = sync_ticks;
		move_snake(current_movement);

	}
	draw_snake();

	if (is_eating_apple() || !apple_spawned()) {
		spawn_apple();
	}
	kputs("@", make_color(LIGHT_RED, BLACK, 0), apple.x, apple.y);

}

void reset_game() {
	head = snake + 1;
	tail = snake;
	apple.x = 0;
	apple.y = 0;
	head->x = 40;
	head->y = 12;
	tail->x = 0;
	tail->y = 0;
	current_movement = MOVE_NONE;
	last_ticked_movement = MOVE_NONE;
	current_tick = 0;
}
