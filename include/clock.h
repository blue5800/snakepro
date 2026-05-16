#include <context.h>
#include <stdint.h>
#define TARGET_FREQ 100
uint32_t get_ticks();
void init_timer();
void handle_timer_interrupt(struct registers *regs);
