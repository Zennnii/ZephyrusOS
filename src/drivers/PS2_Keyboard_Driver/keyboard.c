#include "keyboard.h"

bool shift_pressed = false;

char scancode_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '\n', 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*',
    0, ' ', 0, 0, 0, 0, 0, 0, 0
};

// Shifted versions for keys that change with shift pressed
char scancode_shift_map[128] = {
    0, 27, '!', '@', '#', '$', '%', '^',
    '&', '*', '(', ')', '_', '+', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
    'O', 'P', '{', '}', '\n', 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
    '"', '~', 0, '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', 0, '*',
    0, ' ', 0, 0, 0, 0, 0, 0, 0
};

void keyboard_isr_handler() {
    static bool shift_pressed = false;

    uint8_t scancode = inb(KBD_DATA_PORT);

    bool key_released = scancode & 0x80;
    uint8_t keycode = scancode & 0x7F;

    if (keycode == 0x2A || keycode == 0x36) {
        shift_pressed = !key_released;
        return;
    }

    if (!key_released) {
        char c = shift_pressed ? scancode_shift_map[keycode] : scancode_map[keycode];
        if (c) {
            if (c == '\b') {
                backspace();
            } else {
                print_char(c);
            }
        }
    }
}