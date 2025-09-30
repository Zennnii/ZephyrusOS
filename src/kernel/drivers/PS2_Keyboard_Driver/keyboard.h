#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "util/util.h"
#include "stdint.h"

#define KBD_DATA_PORT 0x60
#define KEYBOARD_BUFFER_SIZE 128

extern char key_buffer[KEYBOARD_BUFFER_SIZE];
extern int key_buffer_head;
extern int key_buffer_tail;

bool buffer_is_empty(void);
bool buffer_is_full(void);

void init_keyboard(void);
void keyboard_isr_handler(void);
char keyboard_getchar(void);
void buffer_flush(void);

extern char scancode_map[128];
extern char scancode_shift_map[128];

#endif