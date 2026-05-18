#include <string.h>
#include <stdint.h>
#include <io.h>
#include <keyboard.h>
#include <vga.h>
#include <stdlib.h>
#include <movement.h>
#include <game.h>

#define KEY_RELEASE_MASK 0x80
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

static char val_str_buffer[32] = {0};

/*
* problem: basically when we adjust the ticks downwards. we get a situation where the saved one was calculated with the higher multiplier
* this pauses the game, dependent on the already elapsed time, until the ticks with the new mulitplier catches up. need to scale the current value to accomodate
* let x be the variable current ticks, M the multiplier, t the current time when it was taken, and T be the TARGET_FREQ:
* x = t * M / T -> t = x * T / M
* by adjusting M by adding delta d we get x' = t * (M + d) / T
* we need to find x' in terms of x, M, and d. 
* subsituting t we get x' = (m + d) *xT/MT = x * (M + d) / M
* this is all the simplification that's needed really 
*/
static inline void renormalise_game_tick(int8_t delta){
    current_tick = (current_tick * (game_speed_multiplier + delta)) / game_speed_multiplier;
}
/*
 * we have the whole table, but we only need wasd, p, up, down, and r.
 * wasd for movement, up/down for gamespeed, p for pause and r for reset.
 * since we aren't typing, we can use raw scancodes.
 */
uint8_t key_pressed[128];
uint8_t last_pressed_key;

uint8_t pause_wait_for_reset = 0;
uint8_t pause_state = 0;

enum MOVEMENT current_movement = MOVE_NONE;

void handle_keyboard_interrupt(struct registers *regs){

        uint8_t status = inb(KEYBOARD_STATUS_PORT);
        if (status & 0x01) { // check if output buffer is full
            uint8_t scancode = inb(KEYBOARD_DATA_PORT);
            if ((scancode & KEY_RELEASE_MASK) == 0) {
                key_pressed[scancode] = 1;
                last_pressed_key = scancode;
                
                if ((scancode & ~KEY_RELEASE_MASK) == KEY_R) {
                    reset_game();
                    pause_state = 0;
                    pause_wait_for_reset = 0;
                }

                if (scancode == KEY_P && !pause_wait_for_reset) {
                    pause_wait_for_reset = 1;
                    pause_state = !pause_state; // toggle pause state
                }
                
                //up and down keys adjust game speed multiplier
                if (scancode == KEY_UP) {
                    if (game_speed_multiplier < 25) {
                        game_speed_multiplier += 1;
                        renormalise_game_tick(1);
                    }
                }
                else if (scancode == KEY_DOWN) {
                    if (game_speed_multiplier > 4) {
                        game_speed_multiplier -= 1;
                        renormalise_game_tick(-1);
                    }
                }

                //handle movement keys
                if (scancode == KEY_W && current_movement != MOVE_DOWN) {
                    current_movement = MOVE_UP;
                }
                else if (scancode == KEY_A && current_movement != MOVE_RIGHT) {
                    current_movement = MOVE_LEFT;
                }
                else if (scancode == KEY_S && current_movement != MOVE_UP) {
                    current_movement = MOVE_DOWN;
                }
                else if (scancode == KEY_D && current_movement != MOVE_LEFT) {
                    current_movement = MOVE_RIGHT;
                }

            }
            else {
                key_pressed[scancode & ~KEY_RELEASE_MASK] = 0;

                if ((scancode & ~KEY_RELEASE_MASK) == KEY_P) {
                    pause_wait_for_reset = 0;
                }
            }
        }

        outb(0x20, 0x20); // send End of Interrupt (EOI) signal to PIC
        
        return;	
}

uint8_t is_paused(){
    return pause_state;
}

