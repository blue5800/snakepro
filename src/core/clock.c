#include <clock.h>
#include <string.h>
#include <stdlib.h>
#include <vga.h>
#include <io.h>
static char timer[32] = {0};
void handle_timer_interrupt(struct registers *regs) {
	static int timer_tick_count = 0;
	memset(timer, 0, 32);
	itoa(++timer_tick_count,timer);
	kputs(timer, make_color(LIGHT_CYAN, BLACK, 0), 0, 20);
	outb(0x20, 0x20); 
	return;
}
