#include <string.h>
#include <stdint.h>
#include <io.h>
#include <keyboard.h>
#include <vga.h>
#include <stdlib.h>

#define KEY_RELEASE_MASK 0x80
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

static char val_str_buffer[32] = {0};

/*
 * we have the whole table, but we only need wasd, p, up, down, and r.
 * wasd for movement, up/down for gamespeed, p for pause and r for reset.
 * since we aren't typing, we can use raw scancodes.
 */
uint8_t key_pressed[128];
uint8_t last_pressed_key;

uint8_t pause_wait_for_reset = 0;
uint8_t pause_state = 0;

void handle_keyboard_interrupt(struct registers *regs){

        uint8_t status = inb(KEYBOARD_STATUS_PORT);
        if (status & 0x01) { // check if output buffer is full
            uint8_t scancode = inb(KEYBOARD_DATA_PORT);
            if ((scancode & KEY_RELEASE_MASK) == 0) {
                key_pressed[scancode] = 1;
                last_pressed_key = scancode;
                
                if (scancode == KEY_P && !pause_wait_for_reset) {
                    pause_wait_for_reset = 1;
                    pause_state = !pause_state; // toggle pause state
                }
            }
            else {
                key_pressed[scancode & ~KEY_RELEASE_MASK] = 0;

                if ((scancode & ~KEY_RELEASE_MASK) == KEY_P) {
                    pause_wait_for_reset = 0;
                }
            }

            kputs("                                     ", make_color(LIGHT_GREEN, BLACK, 0), 20, 20); //dirty hack to clear previous scancode
            kputs(" last pressed: 0x", make_color(LIGHT_GREEN, BLACK, 0), 23, 20);
            itoh(last_pressed_key, val_str_buffer);
            kputs(val_str_buffer, make_color(LIGHT_GREEN, BLACK, 0), 40, 20);
        }

        outb(0x20, 0x20); // send End of Interrupt (EOI) signal to PIC
        
        return;	
}

uint8_t is_paused(){
    return pause_state;
}

