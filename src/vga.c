#include "vga.h"
#include "util.h"

uint16_t column = 0;
uint16_t line = 0;
uint16_t* const vga = (uint16_t* const) 0xB8000;
const uint16_t defaultColor = (COLOR8_WHITE << 8) | (COLOR8_BLACK << 12);
uint16_t currentColor = defaultColor;


uint16_t get_cursor_offset() {
    // Ask VGA controller for high byte of cursor pos
    outb(VGA_CONTROL_PORT, 0x0E);
    uint16_t offset = inb(VGA_DATA_PORT) << 8;

    // Ask for low byte of cursor pos
    outb(VGA_CONTROL_PORT, 0x0F);
    offset |= inb(VGA_DATA_PORT);

    return offset;
}

void get_cursor_position(int* x, int* y) {
    uint16_t offset = get_cursor_offset();
    *x = offset % width;
    *y = offset / width;
}

void moveCursor(uint16_t row, uint16_t col) {
    uint16_t pos = row * width + col;

    // Tells VGA we want to set cursor high byte
    outb(VGA_CONTROL_PORT, 0x0E);
    outb(VGA_DATA_PORT, (pos >> 8) & 0xFF);

    // Tells VGA we want to set cursor low byte
    outb(VGA_CONTROL_PORT, 0x0F);
    outb(VGA_DATA_PORT, pos & 0xFF);
}

void Reset() {
    line = 0;
    column = 0;

    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            vga[y * width + x] = ' ' | defaultColor;
        }
    }
    moveCursor(line, column);
}

void scrollUp() {
    for (uint16_t y = 1; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            vga[(y-1) * width + x] = vga[y * width + x];
        }
    }
        for (uint16_t x = 0; x < width; x++) {
            vga[(height - 1) * width + x] = ' ' | defaultColor;
        }
        moveCursor(line, column);
    }

void newLine() {
    if (line < height - 1) {
        line++;
        column = 0;
    }
    else {
        scrollUp();
        column = 0;
    }
    moveCursor(line, column);
}

void print_char(char c) {
    switch(c) {
        case '\n':
            newLine();
            break;
        case '\r':
            column = 0;
            break;
        default:
            if (column == width) {
                newLine();
            }
            vga[line * width + (column++)] = c | currentColor;
            break;
    }
    moveCursor(line, column);
}

void print(const char* s) {
    while (*s) {
        print_char(*s++);
    }
    moveCursor(line, column);
}

void print_hex(uint16_t val) {
    const char *hex = "0123456789ABCDEF";
    char buf[5];
    buf[4] = '\0';

    for (int i = 3; i >= 0; i--) {
        buf[i] = hex[val & 0xF];
        val >>= 4;
    }
    print(buf);
    moveCursor(line, column);
}

void print_dec(unsigned int num) {
    if (num == 0) {
        print("0");
        return;
    }

    char buffer[10]; // Enough for 32-bit integer decimal digits
    int i = 0;

    // Extract digits in reverse order
    while (num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    // Print digits in correct order
    for (int j = i - 1; j >= 0; j--) {
        print_char(buffer[j]);
    }
    moveCursor(line, column);
}

void backspace() {
    if (column > 0) {
        column--;
        vga[line * width + column] = ' ' | currentColor; // Erase char
        moveCursor(line, column);
    }
    // else if column == 0, do nothing (no backspacing into previous line)
}