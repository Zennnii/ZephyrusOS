#include "logf.h"

void klog(const char* level, char* msg, uint32_t color, uint8_t nlflag) {
    draw_string(fb, fb_width, 0, curLine, "[ ", 0xFFFFFFFF);
    draw_string(fb, fb_width, curX, curLine, level, color);
    draw_string(fb, fb_width, curX, curLine, " ] ", 0xFFFFFFFF);
    print_time_log();
    draw_string(fb, fb_width, curX, curLine, ":", 0xFFFFFFFF);
    draw_string(fb, fb_width, curX, curLine, msg, 0xFFFFFFFF);

    if (nlflag == 1) {
    newLineFB();
    }
}