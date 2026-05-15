#include <vga.h>
#include <stdint.h>
#include <io.h>

struct registers {
    unsigned int ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
};
static char val_str_buffer[32] = {0};
static char timer[32] = {0};
static int timer_tick_count = 0;
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

static void memset(void *ptr, char value, int num){
    char *cptr = (char*)ptr;
    for (int i = 0; i < num; i++) {
        cptr[i] = value;
    }
}

void handle_interrupt(struct registers *regs) {
    // for now, just print the interrupt number and halt the system.
    // todo: implement the actual dispatch logic
    
    static int keyboard_interrupt_count = 0;
    if (regs->int_no == 32) {
        //display time in bottom left corner
        memset(timer, 0, 32);
        itoa(++timer_tick_count,timer);
        kputs(timer, make_color(LIGHT_CYAN, BLACK, 0), 0, 20);
        return;
    }
    memset(timer, 0, 32);

    // do something with the keyboard interrupt
    if (regs->int_no == 33) {
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
    
    kputs("interrupt number: ", make_color(LIGHT_RED, BLACK, 0),20 , 20);
    itoa(regs->int_no, val_str_buffer);
    kputs(val_str_buffer, make_color(LIGHT_RED, BLACK, 0), 50, 20);
    //put_char('0' + regs->err_code, make_color(LIGHT_RED, BLACK, 0), 20, 0);
}
