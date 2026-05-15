#include <string.h>
#include <stdint.h>
#include <io.h>
#include <keyboard.h>
#include <vga.h>
#include <stdlib.h>

static char val_str_buffer[32] = {0};
static int keyboard_interrupt_count = 0;

void keyboard_init() {
	// unmask irq1 on PIC, to allow interrupts to be recieved.
	uint8_t mask = inb(0x21);
	mask &= ~(1 << 1); // Unmask IRQ1 (keyboard)
	outb(0x21, mask);

	//communicate to PS/2 controller, send 0x20 to port 0x64. set bit 0, write 0x60 back
	outb(0x64, 0x20); // send command to read controller configuration byte
	uint8_t config = inb(0x60); // read the configuration byte
	outb(0x64, 0x60); // send command to write controller configuration byte
	outb(0x60, config & 0b1); // clear bit 4 to enable keyboard interrupts
	
}

void handle_keyboard_interrupt(struct registers *regs){
	memset(val_str_buffer, 0, 32);
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

