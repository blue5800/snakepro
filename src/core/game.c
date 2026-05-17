#include <game.h>
#include <stdint.h>
#include <stdlib.h>
struct Point apple = {0,0};

// our snake can have a max length of (78*22) - 1 = 1715 
#define MAX_ROWS 22
#define MAX_COLS 78
#define MAX_SNAKE_LENGTH (MAX_ROWS * MAX_COLS - 1)
struct Point snake[1715] = {{0,0},{40, 12}};

struct Point *head = (snake + 1);
struct Point *tail = snake;

static inline uint8_t apple_spawned() {
	return apple.x != 0 || apple.y != 0;
}

static inline void spawn_apple() {
retry:
	apple.x = (rand() % 78) + 1;
	apple.y = (rand() % 22) + 1;
	//make sure we're not touching the snake with the apple
	struct Point *current = tail;
	while (current != head) {
		if (current->x == apple.x && current->y == apple.y) {
			goto retry;
		}
		current = snake + ((current - snake + 1) % 1715);
	}
	if (head->x == apple.x && head->y == apple.y) {
		goto retry;
	}

}

static inline uint8_t is_eating_apple() {
	if (head->x == apple.x && head->y == apple.y) {
		return 1;
	}
	return 0;
}

static inline uint8_t is_out_of_bounds() {
	if (head->x <= 0 || head->x >= 79 || head->y <= 0 || head->y >= 24) {
		return 1;
	}
	return 0;
}

static inline uint8_t snake_intersects_self() {
	if ((head - tail) == 1) return 0;
	struct Point *start = tail;
	while (start != head) {
		if (head->x == start->x && head->y == start->y) {
			return 1;
		}
		start = snake + ((start - snake + 1) % 1715);
	}
	return 0;
}

static inline void update_head() {
	struct Point *new_head = snake + ((head - snake + 1) % 1715);
	new_head->x = head->x;
	new_head->y = head->y;
	head = new_head;
}

void move_snake(enum MOVEMENT dir) {
	// when we move the snake, simply enqueue a new head, and if we are not growing, dequeue the tail
	update_head();

	switch (dir) {
	case MOVE_UP:
		head->x = head[-1].x;
		head->y = head[-1].y - 1;
		break;
	case MOVE_DOWN:
		head->x = head[-1].x;
		head->y = head[-1].y + 1;
		break;
	case MOVE_LEFT:
		head->x = head[-1].x - 1;
		head->y = head[-1].y;
		break;
	case MOVE_RIGHT:
		head->x = head[-1].x + 1;
		head->y = head[-1].y;
		break;
	}
	if (!is_eating_apple()) {
		tail = snake + ((tail - snake + 1) % 1715);
	}
}

void draw_snake() {
	struct Point *current = head;
	while (current != tail) {
		kputs("O", make_color(LIGHT_GREEN, BLACK, 0), current->x, current->y);
		current = snake + ((current - snake - 1 + 1715) % 1715);
	}
}

uint32_t current_tick = 0;

void update_game_state() {
	if (is_out_of_bounds() || snake_intersects_self()) {
		kputs("loser", make_color(LIGHT_RED, BLACK, 0), 35, 12);
		return;	
	}

	if (get_ticks()*10 / TARGET_FREQ > current_tick && !is_paused()) {
		current_tick = get_ticks()*10 / TARGET_FREQ;
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
}
