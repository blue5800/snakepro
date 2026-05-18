#include <movement.h>
#include <clock.h>
#include <vga.h>
#include <keyboard.h>
struct Point {
	int x;
	int y;
};

// for now, i just want a moving snake lmao
extern enum MOVEMENT last_ticked_movement;
extern uint32_t game_speed_multiplier;
extern uint32_t current_tick;

void update_game_state();
void reset_game();


/*
* problem: basically when we adjust the ticks downwards. we get a situation where the saved one was calculated with the higher multiplier
* this pauses the game, dependent on the already elapsed time, until the ticks with the new mulitplier catches up. need to scale the current value to accomodate
* let x be the variable current ticks, M the multiplier, t the current time when it was taken, and T be the TARGET_FREQ:
* x = t * M / T -> t = x * T / M
* by adjusting M by adding delta d we get x' = t * (M + d) / T
* we need to find x' in terms of x, M, and d. 
* subsituting t we get x' = (m + d) *xT/MT = x * (M + d) / M
* this is all the simplification that's needed really
* one issue we still have is that it doesn't reset the timestep so the first tick might feel a bit off
* good enough. 
*/
static inline void renormalise_game_tick(int8_t delta){
    current_tick = (current_tick * (game_speed_multiplier + delta)) / game_speed_multiplier;
}
