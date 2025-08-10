#include "vga.h"

uint16_t column = 0;
uint16_t line = 0;
uint16_t* const vga = (uint16_t* const) 0xB8000;
const uint16_t defaultColor = (COLOR8_WHITE << 8) | (COLOR8_BLACK << 12);
uint16_t currentColor = defaultColor;

void Reset() {
    line = 0;
    column = 0;

    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            vga[y * width + x] = ' ' | defaultColor;
        }
    }
}

void scrollUp() {
    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            vga[(y-1) * width + x] = vga[y * width + x];
        }
    }
        for (uint16_t x = 0; x < width; x++) {
            vga[(height - 1) * width + x] = ' ' | defaultColor;
        }
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
}

void print(const char* s) {
    while(*s) {
        switch(*s) {
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

                vga[line * width + (column++)] = *s | currentColor;
        }
        s++;
        
    }
}

void printHex(uint16_t val) {
    const char *hex = "0123456789ABCDEF";
    char buf[5];
    buf[4] = '\0';

    for (int i = 3; i >= 0; i--) {
        buf[i] = hex[val & 0xF];
        val >>= 4;
    }
    print(buf);
}