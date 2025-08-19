#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "util.h"
#include "stdint.h"
#include "vga.h"

#define KBD_DATA_PORT 0x60
#define KEY_UP    0x80
#define KEY_DOWN  0x81
#define KEYBOARD_BUFFER_SIZE 128

extern char key_buffer[KEYBOARD_BUFFER_SIZE];
extern int key_buffer_head;
extern int key_buffer_tail;

bool buffer_is_empty();
bool buffer_is_full();

void init_keyboard();
void keyboard_isr_handler();
char keyboard_getchar();
void buffer_flush(void);

extern char scancode_map[128];
extern char scancode_shift_map[128];

#endif