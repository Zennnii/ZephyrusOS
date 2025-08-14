#include "bcmds.h"

char echoIn[LINE_BUFFER_SIZE];

void echof(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        print(argv[i]);
        print(" ");
    }
    newLine();
}

void clearf() {
    Reset();
}

void verf() {
    print("Zephyrus OS [Version 1.0.0]");
    newLine();
}

void helpf() {
    print("echo: Prints a message\n");
    print("clear: Clears the screen\n");
    print("ver: Tells the current version of Zephyrus OS\n");
    print("help: Shows this message\n");
    print("shutdown: Halts the CPU and allows you to turn of your PC safely (no ACPI yet)\n");
}

void shutdownf() {
    wait(5);
    Reset();
    wait(2);

    print("It's now safe to turn off your computer\n");
    vga_hide_cursor();

    disableInterrupts();
    for (;;) {
        asm volatile("hlt");
    }
}