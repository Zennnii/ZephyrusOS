#include "logf.h"

void klog(const char* level, char* msg, uint16_t color, uint8_t nlflag) {
    printcol("[ ", COLOR8_WHITE);
    printcol(level, color);
    printcol(" ] ", COLOR8_WHITE);
    print_time_log();
    printcol(":", COLOR8_WHITE);
    print(msg);

    if (nlflag == 1) {
    newLine();
    }
}