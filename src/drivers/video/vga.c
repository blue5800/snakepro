#include <vga.h>
volatile unsigned short *VGA_TXT_MEMORY_ADDR = (volatile unsigned short *)0xB8000;

//note: we will be making snake game so we're gonna have to switch to mode 13h at some point...
volatile unsigned char *VGA_GRAPHICS_MEMORY_ADDR = (volatile unsigned char *)0xA0000;

char make_color(enum VGA_COLOR fg, enum VGA_COLOR bg, int blink){
	return fg | bg << 4 | (blink ? 1 << 7 : 0);
}
void put_char(char c, char color, int x, int y) {
	const int index = y * 80 + x;
	VGA_TXT_MEMORY_ADDR[index] = (color << 8) | c;
}
void clear_screen(char color) {
	for (int y = 0; y < 25; y++) {
		for (int x = 0; x < 80; x++) {
			put_char(' ', color, x, y);
		}
	}
}

void kputs(const char* str, char color, int x, int y) {
	while (*str) {
		put_char(*str++, color, x++, y);
		if (x >= 80) {
			x = 0;
			y++;
		}
		if (y >= 25) {
			y = 0;
		}
	}
}

