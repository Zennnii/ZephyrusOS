#include "panic.h"

void kernelPanic(const char* errorMessage, uint32_t errorCode) {
    clear(0x960b0b);
    // Print panic header
    draw_string(fb, fb_width, 0, 0, "KERNEL PANIC\n", colorWhite);
    // Print error message
    draw_string(fb, fb_width, curX, curLine, "ERROR: ", colorWhite);
    draw_string(fb, fb_width, curX, curLine, errorMessage, colorWhite);
    newLineFB();

    // Print error code in hex
    draw_string(fb, fb_width, curX, curLine, "ERROR_CODE: 0x", colorWhite);
    draw_hex(errorCode);
    newLineFB();
    newLineFB();

    draw_string(fb, fb_width, curX, curLine, ":(", colorWhite);

    disableInterrupts();

    // Halt CPU forever
    while (1) {
        __asm__ volatile ("hlt");
    }
}