#include <game.h>
#include <stdint.h>
#include <stdlib.h>
struct Point snake = {40, 12};
struct Point apple = {0,0};

static inline uint8_t apple_spawned() {
	return apple.x != 0 || apple.y != 0;
}

static inline void spawn_apple() {
	apple.x = (rand() % 78) + 1;
	apple.y = (rand() % 22) + 1;
}

static inline uint8_t is_eating_apple() {
	if (snake.x == apple.x && snake.y == apple.y) {
		return 1;
	}
	return 0;
}

static inline uint8_t is_out_of_bounds() {
	if (snake.x <= 0 || snake.x >= 79 || snake.y <= 0 || snake.y >= 23) {
		return 1;
	}
	return 0;
}

uint32_t current_tick = 0;

void update_game_state() {

	if (get_ticks()*10 / TARGET_FREQ > current_tick && !is_paused()) {
		current_tick = get_ticks()*10 / TARGET_FREQ;

		kputs(" ", make_color(BLACK, BLACK, 0), snake.x, snake.y);
		switch (current_movement) {
			case MOVE_UP:
				snake.y--;
				break;
			case MOVE_DOWN:
				snake.y++;
				break;
			case MOVE_LEFT:
				snake.x--;
				break;
			case MOVE_RIGHT:
				snake.x++;
				break;
			default:
				break;
		}
	}
	if (is_out_of_bounds()) {
		kputs("loser", make_color(LIGHT_RED, BLACK, 0), 35, 12);
		return;	
		//snake.x = 40;
		//snake.y = 12;
	}
	kputs("O", make_color(LIGHT_GREEN, BLACK, 0), snake.x, snake.y);

	if (is_eating_apple()) {
		apple.x = 0;
		apple.y = 0;
	}

	if (!apple_spawned()) {
		spawn_apple();
	}

	kputs("@", make_color(LIGHT_RED, BLACK, 0), apple.x, apple.y);

}
