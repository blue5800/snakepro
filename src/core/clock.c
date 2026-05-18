#include <clock.h>
#include <string.h>
#include <stdlib.h>
#include <vga.h>
#include <io.h>
#include <keyboard.h>
static char timer[32] = {0};

uint32_t ticks = 0;

uint32_t get_ticks() {
	return ticks;
}

/*
 * note: the x86 PIT has an input frequency of 1193182hz
 * we can change it by sending a divisor = 1193182/target_freq to io port 0x43 data port 0x40
 */
#define PIT_INPUT_FREQ 1193182
void init_timer() {
	uint16_t divisor = PIT_INPUT_FREQ / TARGET_FREQ;
	outb(0x43, 0x36); // command byte: channel 0, access mode lobyte/hibyte, mode 3 (square wave), binary mode
	outb(0x40, divisor & 0xFF); // send low byte of divisor
	outb(0x40, (divisor >> 8) & 0xFF); // send high byte of divisor
}

void handle_timer_interrupt(struct registers *regs) {
	if (!is_paused()) ++ticks;
	outb(0x20, 0x20);
	return;
}


