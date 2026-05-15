#include <idt.h>
#include <vga.h>
void kernel_main(){
    idt_install();
//kputs("is this thing on?", 0x1F, 0, 0);
    int x = x / 0;
    while(1);
}
