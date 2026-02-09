#include "logf.h"
#include "drivers/cmos_rtc/cmos_rtc.h"
#include "drivers/com_1/com_1.h"
#include "gfx.h"

void klog(const char* level, char* msg, uint32_t color, uint8_t nlflag) {
    serial_write("[ ");
    serial_write(level);
    serial_write(" ] ");
    serial_write_time_log();
    serial_write(" : ");
    serial_write(msg);

    if (nlflag == true) {
        serial_write_char('\n');
    }
}

void printKlog(const char* level, char* msg, uint32_t color, uint8_t nlflag) {
    draw_string(fb, fb_width, curX, curLine, "[ ", colorWhite);
    draw_string(fb, fb_width, curX, curLine, level, colorWhite);
    draw_string(fb, fb_width, curX, curLine, " ] ", colorWhite);
    print_time_log();
    draw_string(fb, fb_width, curX, curLine, " : ", colorWhite);
    draw_string(fb, fb_width, curX, curLine, msg, colorWhite);

    if (nlflag == true) {
        newLineFB();
    }
}