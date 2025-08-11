#ifndef VGA_H
#define VGA_H

#include "stdint.h"

#define COLOR8_BLACK 0
#define COLOR8_BLUE 1
#define COLOR8_GREEN 2
#define COLOR8_CYAN 3
#define COLOR8_RED 4
#define COLOR8_MAGENTA 5
#define COLOR8_BROWN 6
#define COLOR8_LIGHT_GRAY 7
#define COLOR8_DARK_GRAY 8
#define COLOR8_LIGHT_BLUE 9
#define COLOR8_LIGHT_GREEN 10
#define COLOR8_LIGHT_CYAN 11
#define COLOR8_LIGHT_RED 12
#define COLOR8_LIGHT_MAGENTA 13
#define COLOR8_LIGHT_BROWN 14
#define COLOR8_WHITE 15

#define width 80
#define height 25

#define VGA_CONTROL_PORT 0x3D4
#define VGA_DATA_PORT    0x3D5

void print(const char* s);
void print_char(char c);
void print_hex(uint16_t val);
void print_dec(unsigned int num);
void scrollUp();
void newLine();
void Reset();
void backspace();
void moveCursor(uint16_t row, uint16_t col);
void get_cursor_position(int* x, int* y);
uint16_t get_cursor_offset();

#endif