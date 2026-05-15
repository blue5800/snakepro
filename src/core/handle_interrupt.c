#include <vga.h>


struct registers {
    unsigned int ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
};
static char val_str_buffer[32] = {0};

static void itoa(int value, char* buffer) {
    int i = 0;
    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }
    while (value > 0 && i < 31) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }
    buffer[i] = '\0';
    // reverse the string
    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }
}

void handle_interrupt(struct registers *regs) {
    // for now, just print the interrupt number and halt the system.
    // todo: implement the actual dispatch logic
    kputs("interrupt number: ", make_color(LIGHT_RED, BLACK, 0),20 , 20);
    itoa(regs->int_no, val_str_buffer);
    kputs(val_str_buffer, make_color(LIGHT_RED, BLACK, 0), 50, 20);
    //put_char('0' + regs->err_code, make_color(LIGHT_RED, BLACK, 0), 20, 0);
}
