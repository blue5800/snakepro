#include <context.h>
#include <stdint.h>
enum KEYBOARD_KEYS {
	KEY_W = 0x11,
	KEY_A = 0x1E,
	KEY_S = 0x1F,
	KEY_D = 0x20,
	KEY_P = 0x19,
	KEY_R = 0x13,
	KEY_UP = 0x48,
	KEY_DOWN = 0x50
	
};
extern uint8_t key_pressed[128];
extern uint8_t last_pressed_key;
uint8_t is_paused();
void handle_keyboard_interrupt(struct registers *regs);
