#include <idt.h>
#include <vga.h>
#include <clock.h>
#include <keyboard.h>

void kernel_main(){
    init_timer(); 
    idt_install();
    kputs("is this thing on?", 0x1F, 0, 0);
    while(1);
}
