#include <game.h>
#include <stdint.h>
struct Point snake = {40, 12};
uint32_t current_tick = 0;
void update_game_state() {
	if (get_ticks() / 10*TARGET_FREQ > current_tick && !is_paused()) {
		current_tick = get_ticks() / 10*TARGET_FREQ;

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
	kputs("O", make_color(LIGHT_GREEN, BLACK, 0), snake.x, snake.y);
}
