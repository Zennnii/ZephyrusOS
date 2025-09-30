#include "bmcmds.h"

void load_main_cli() {
    bare_cli_running = false;

    cli();
}

void helpb() {
    draw_string(fb, fb_width, 0, curLine, "load-main-cli: Loads the main kernel CLI\n", colorWhite);
    draw_string(fb, fb_width, 0, curLine, "shutdown: Halts the CPU and allows you to turn of your PC safely (no ACPI yet)\n", colorWhite);
    draw_string(fb, fb_width, 0, curLine, "reboot: Reboots the computer\n", colorWhite);
}