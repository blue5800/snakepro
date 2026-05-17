#include <movement.h>
#include <clock.h>
#include <vga.h>
#include <keyboard.h>
struct Point {
	int x;
	int y;
};

// for now, i just want a moving snake lmao
extern uint32_t game_speed_multiplier;
void update_game_state();
void reset_game();
