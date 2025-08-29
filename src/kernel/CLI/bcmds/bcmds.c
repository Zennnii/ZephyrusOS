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
    print("Zephyrus OS [Version 1.0.8]");
    newLine();
}

void helpf() {
    print("echo <message>: Prints a message\n");
    print("clear: Clears the screen\n");
    print("ver: Shows the current version of Zephyrus OS\n");
    print("help: Shows this message\n");
    print("shutdown: Halts the CPU and allows you to turn of your PC safely (no ACPI yet)\n");
    print("reboot: Reboots the computer\n");
    print("uptime: Prints system uptime\n");
    print("panic: Initiates a test kernel panic\n");
    print("colors: Prints all the colors available\n");
    print("time: Prints current date and time\n");
    print("exit: Exits kernel CLI (falls back to bare minimum CLI)\n");
}

void shutdownf() {
    wait(3);
    Reset();
    wait(2);

    Reset();
    print("It's now safe to turn off your computer\n");
    vga_hide_cursor();

    // Disables interrupts (cli)
    disableInterrupts();

    // Halts indefinitely
    for (;;) {
        asm volatile("hlt");
    }
}

// Reboots the computer via 8042
void reboot8042() {
    cli_running = false;
    wait(2);
    Reset();
    wait(3);
    print("Rebooting...");
    vga_hide_cursor();
    wait(2);

    // Pulse the reset line through the keyboard controller
    outb(0x64, 0xFE); // 8042 reset command

    // If rebooting fails, halt indefinitely
    while (1) {
        __asm__ volatile("hlt");
    }

}

void uptimef() {
    print_dec(sys_uptime);
    newLine();
}

void panicf() {
    print("This command will initiate a kernel panic are you sure [y/n]: ");
    getline(userIn);
    if (strcmp(userIn, "y") == 0) {
        wait(2);
        kernelPanic("TEST KERNEL PANIC TRIGGERED", 0xDEADBEEF);
    }
}

void colorsf() {
    printcol("Blue ", COLOR8_BLUE);
    printcol("Brown ", COLOR8_BROWN);
    printcol("Cyan\n", COLOR8_CYAN);
    printcol("Dark Gray ", COLOR8_DARK_GRAY);
    printcol("Green ", COLOR8_GREEN);
    printcol("Light Blue\n", COLOR8_LIGHT_BLUE);
    printcol("Light Brown ", COLOR8_LIGHT_BROWN);
    printcol("Light Cyan ", COLOR8_LIGHT_CYAN);
    printcol("Light Gray\n", COLOR8_LIGHT_GRAY);
    printcol("Light Green ", COLOR8_LIGHT_GREEN);
    printcol("Light Magenta ", COLOR8_LIGHT_MAGENTA);
    printcol("Light Red\n", COLOR8_LIGHT_RED);
    printcol("Magenta ", COLOR8_MAGENTA);
    printcol("Red ", COLOR8_RED);
    printcol("White\n", COLOR8_WHITE);
}

void timef() {
    print_date_time();
}

void exitf() {
    LOG_INFO("Exiting kernel CLI...");
    wait(1);
    LOG_INFO("Entering bare minimum CLI...");
    wait(2);
    Reset();
    cli_running = false;
    bare_cli();
}