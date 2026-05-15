#include <idt.h>
#include <vga.h>
#include <keyboard.h>
void kernel_main(){
    idt_install();
    //keyboard_init();
    kputs("is this thing on?", 0x1F, 0, 0);
    //int x = x / 0;
    while(1);
}
