#include <stdint.h>
#include <io.h>
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

