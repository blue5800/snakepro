#include <idt.h>
#include <vga.h>
#include <clock.h>
#include <keyboard.h>
#include <stdlib.h>
#include <movement.h>

char uptime_str[32] = {0};
void kernel_main(){
    init_timer(); 
    idt_install();
    while(1){
        draw_border();
        kputs("uptime (s): ",  make_color(LIGHT_CYAN, BLACK, 0) , 0, 0);
        uint32_t uptime = get_ticks() / TARGET_FREQ;
        itoa(uptime, uptime_str);
        kputs(uptime_str, make_color(LIGHT_CYAN, BLACK, 0), 12, 0);

        if (is_paused()) {
            kputs("PAUSED", make_color(LIGHT_RED, BLACK, 0), 74, 0);
        }
        else {
            //kputs("      ", make_color(LIGHT_CYAN, BLACK, 0), 74, 0); 
        }

        switch (current_movement) {
            case MOVE_UP:
                kputs("UP   ", make_color(LIGHT_GREEN, BLACK, 0), 0, 1);
                break;
            case MOVE_DOWN:
                kputs("DOWN ", make_color(LIGHT_GREEN, BLACK, 0), 0, 1);
                break;
            case MOVE_LEFT:
                kputs("LEFT ", make_color(LIGHT_GREEN, BLACK, 0), 0, 1);
                break;
            case MOVE_RIGHT:
                kputs("RIGHT", make_color(LIGHT_GREEN, BLACK, 0), 0, 1);
                break;
            default:
                kputs("NONE ", make_color(LIGHT_GREEN, BLACK, 0), 0, 1);
        }
        render_frame();
    }
}
