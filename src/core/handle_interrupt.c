#include <vga.h>


struct registers {
    unsigned int ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
};


void handle_interrupt(struct registers *regs) {
    // for now, just print the interrupt number and halt the system.
    // todo: implement the actual dispatch logic
    kputs("iinterrupt number: ", make_color(LIGHT_RED, BLACK, 0),30 , 40);
    put_char('0', make_color(LIGHT_RED, BLACK, 0), 20, 0);
}
