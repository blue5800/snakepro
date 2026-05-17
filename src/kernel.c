#include <idt.h>
#include <vga.h>
#include <clock.h>
#include <keyboard.h>
#include <stdlib.h>
#include <movement.h>
#include <game.h>

char uptime_str[32] = {0};
void kernel_main(){
    init_timer(); 
    idt_install();
    srand(get_ticks());
    while(1){
        clear_screen(make_color(BLACK, BLACK, 0));
        draw_border();

        kputs("uptime (s): ",  make_color(LIGHT_CYAN, BLACK, 0) , 0, 0);
        uint32_t uptime = get_ticks() / TARGET_FREQ;
        itoa(uptime, uptime_str);
        kputs(uptime_str, make_color(LIGHT_CYAN, BLACK, 0), 12, 0);

        if (is_paused()) {
            kputs("PAUSED", make_color(LIGHT_RED, BLACK, 0), 74, 0);
        }
        update_game_state();
        render_frame();
    }
}
