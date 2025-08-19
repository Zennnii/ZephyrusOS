#include "bmcmds.h"

void load_main_cli() {
    bare_cli_running = false;

    cli();
}

void helpb() {
    print("load-main-cli: Loads the main kernel CLI\n");
    print("shutdown: Halts the CPU and allows you to turn of your PC safely (no ACPI yet)\n");
    print("reboot: Reboots the computer\n");
}