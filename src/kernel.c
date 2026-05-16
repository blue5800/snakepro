#include <idt.h>
#include <vga.h>
#include <clock.h>
#include <keyboard.h>
#include <stdlib.h>

char uptime_str[32] = {0};
void kernel_main(){
    init_timer(); 
    idt_install();
    while(1){
        kputs("uptime (s): ",  make_color(LIGHT_CYAN, BLACK, 0) , 0, 0);
        uint32_t uptime = get_ticks() / TARGET_FREQ;
        itoa(uptime, uptime_str);
        kputs(uptime_str, make_color(LIGHT_CYAN, BLACK, 0), 12, 0);

        if (is_paused()) {
            kputs("PAUSED", make_color(LIGHT_RED, BLACK, 0), 30, 0);
        }
        else {
            kputs("      ", make_color(LIGHT_CYAN, BLACK, 0), 30, 0); 
        }
    }
}
