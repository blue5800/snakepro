#include <string.h>
#include <stdint.h>
#include <io.h>
#include <keyboard.h>
#include <vga.h>
#include <stdlib.h>

static char val_str_buffer[32] = {0};
static int keyboard_interrupt_count = 0;

void handle_keyboard_interrupt(struct registers *regs){
        kputs("keyboard interrupt received! num pressed: ", make_color(LIGHT_GREEN, BLACK, 0), 20, 0);
        itoa(++keyboard_interrupt_count, val_str_buffer);
        kputs(val_str_buffer, make_color(LIGHT_GREEN, BLACK, 0), 62, 0);
        
        uint8_t status = inb(0x64);
        
        if (status & 0x01) { // check if output buffer is full
            uint8_t scancode = inb(0x60);
            kputs(" scancode: ", make_color(LIGHT_GREEN, BLACK, 0), 20, 20);
            itoa(scancode, val_str_buffer);
            kputs(val_str_buffer, make_color(LIGHT_GREEN, BLACK, 0), 40, 20);
        }

        outb(0x20, 0x20); // send End of Interrupt (EOI) signal to PIC
        
        return;	
}

