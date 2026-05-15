#include <vga.h>
#include <stdint.h>
#include <io.h>
#include <context.h>
#include <stdlib.h>
#include <keyboard.h>
#include <string.h>
#include <clock.h>

char val_str_buffer[32] = {0};

void handle_interrupt(struct registers *regs) {
/* 
    if (regs->int_no == 32) {
        handle_timer_interrupt(regs);
        return;
    }

    if (regs->int_no == 33) {
        handle_keyboard_interrupt(regs);
        return;
    }
    
    kputs("interrupt number: ", make_color(LIGHT_RED, BLACK, 0),20 , 20);
    itoa(regs->int_no, val_str_buffer);
    kputs(val_str_buffer, make_color(LIGHT_RED, BLACK, 0), 50, 20);
*/ 
    switch (regs->int_no) {
        case 32:
            handle_timer_interrupt(regs);
            break;
        case 33:
            handle_keyboard_interrupt(regs);
            break;
        default:
            kputs("Unhandled interrupt: ", make_color(LIGHT_RED, BLACK, 0), 20, 20);
            itoa(regs->int_no, val_str_buffer);
            kputs(val_str_buffer, make_color(LIGHT_RED, BLACK, 0), 50, 20);
            break;
    }
}
