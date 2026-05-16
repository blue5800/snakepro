#pragma once
/*
* low byte: character to display
* high byte: color (foreground and background). 0-3 are foreground colors, 4-6 are background, 7 is a blink bit.
*/
#include <sys/cdefs.h>

enum VGA_COLOR {
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GREY = 7,
	DARK_GREY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	LIGHT_MAGENTA = 13,
	YELLOW = 14,
	WHITE = 15
};

char make_color(enum VGA_COLOR fg, enum VGA_COLOR bg, int blink);

void put_char(char c, char color, int x, int y);

void clear_screen(char color);
void kputs(const char* str, char color, int x, int y);
void draw_border();
void render_frame();
