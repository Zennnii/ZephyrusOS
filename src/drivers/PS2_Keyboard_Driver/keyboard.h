#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../../util.h"
#include "../../libs/stdint.h"
#include "../../vga.h"

#define KBD_DATA_PORT 0x60

extern bool shift_pressed;

extern void keyboard_isr_handler();
extern char keyboard_getchar();
void buffer_flush(void);

extern char scancode_map[128];
extern char scancode_shift_map[128];

#endif