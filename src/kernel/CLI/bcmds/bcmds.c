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
    print("meminfo: Prints the amount of available system RAM\n");
    print("beep <frequency> <duration>: Plays a certain frequency for a certain duration\n");
    print("music: Plays a short song\n");
    print("rd <file name> <file extension>: Prints the contents of a file\n");
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

void beepf(int argc, char *argv[]) {

    // Convert arguments to integers
    uint32_t freq = atoi(argv[1]);
    uint32_t duration = atoi(argv[2]);

    // Play sound
    speaker_play(freq);

    // Wait duration
    wait(duration);   // your delay function (ms or ticks)
    
    // Stop sound
    speaker_stop();
}

void rdf(int argc, char **argv) {
    
    if (argc < 3) {
        print("Usage: rdf <filename> <ext>\n");
        return;
    }

    uint8_t buffer[4096]; // Enough for small files
    uint32_t size;

    char name[8];
    char ext[3];

    // Fill name with spaces
    memset(name, ' ', 8);
    // Copy up to 8 characters from argv[1]
    for (int i = 0; i < 8 && argv[1][i] != '\0'; i++) {
        name[i] = argv[1][i];
    }

    // Fill ext with spaces
    memset(ext, ' ', 3);
    // Copy up to 3 characters from argv[2]
    for (int i = 0; i < 3 && argv[2][i] != '\0'; i++) {
        ext[i] = argv[2][i];
    }

    for (int i = 0; i < 8 && argv[1][i] != '\0'; i++) {
    char c = argv[1][i];
    if (c >= 'a' && c <= 'z') c -= 32; // to uppercase
    name[i] = c;
    }

    for (int i = 0; i < 3 && argv[2][i] != '\0'; i++) {
        char c = argv[2][i];
        if (c >= 'a' && c <= 'z') c -= 32; // to uppercase
        ext[i] = c;
    }

    if (fat16_read_file(name, ext, buffer, &size)) {
        print(buffer);
        newLine();
    } else {
        print("Error: File not found\n");
    }
}

// Frequency table (approximate, integer Hz)
// -----------------------------
// B-flat major scale notes
// -----------------------------
#define C4 261
#define Cs4 277
#define D4 294
#define Ds4 311    // Eb
#define E4 330
#define F4 349
#define Fs4 370    // Gb
#define G4 392
#define Gs4 415    // Ab
#define A4 440
#define As4 466    // Bb
#define B4 494
#define C5 523
#define Cs5 554
#define D5 587
#define Ds5 622    // Eb5
#define E5 659
#define F5 698
#define Fs5 740    // Gb5
#define G5 784
#define Gs5 831    // Ab5
#define A5 880
#define As5 932    // Bb5
#define B5 988
#define C6 1047

// Durations at approximately 120 BPM
#define WHOLE 2000
#define HALF 1000
#define QUARTER 500
#define EIGHTH 250
#define SIXTEENTH 125
#define DOTTED_QUARTER 750
#define DOTTED_EIGHTH 375

// Small helper pauses
#define SHORT_PAUSE 50
#define MED_PAUSE 100
#define LONG_PAUSE 200

void musicf() {
    init_speaker();
    
    // Measure 1 - Main melody line
    speaker_play(As4); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(C5); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(D5); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(F5); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(D5); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(C5); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(As4); wait_ms(QUARTER); speaker_stop(); wait_ms(SHORT_PAUSE);
    
    // Measure 2 - Melody continuation
    speaker_play(D5); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(F5); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(D5); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(C5); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(As4); wait_ms(QUARTER); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(F5); wait_ms(QUARTER); speaker_stop(); wait_ms(SHORT_PAUSE);
    
    // Measure 3 - Melodic phrase
    speaker_play(G5); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(F5); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(E5); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(D5); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(C5); wait_ms(QUARTER); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(As4); wait_ms(QUARTER); speaker_stop(); wait_ms(SHORT_PAUSE);
    
    // Measure 4 - Second system melody
    speaker_play(F5); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(D5); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(C5); wait_ms(QUARTER); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(G4); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(A4); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(As4); wait_ms(QUARTER); speaker_stop(); wait_ms(SHORT_PAUSE);
    
    // Measure 5 - Melodic development
    speaker_play(D5); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(F5); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(As5); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(F5); wait_ms(EIGHTH); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(D5); wait_ms(QUARTER); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(C5); wait_ms(QUARTER); speaker_stop(); wait_ms(SHORT_PAUSE);
    
    // Measure 6 - Melodic line
    speaker_play(As4); wait_ms(QUARTER); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(G4); wait_ms(QUARTER); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(F4); wait_ms(QUARTER); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(As4); wait_ms(QUARTER); speaker_stop(); wait_ms(SHORT_PAUSE);
    
    // Measure 7 - Phrase continuation
    speaker_play(D5); wait_ms(QUARTER); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(C5); wait_ms(QUARTER); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(As4); wait_ms(QUARTER); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(G4); wait_ms(QUARTER); speaker_stop(); wait_ms(SHORT_PAUSE);
    
    // Measure 8 - Final phrase
    speaker_play(As4); wait_ms(QUARTER); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(C5); wait_ms(QUARTER); speaker_stop(); wait_ms(SHORT_PAUSE);
    speaker_play(F4); wait_ms(HALF); speaker_stop(); wait_ms(MED_PAUSE);
    
    // Final cadence
    speaker_play(As4); wait_ms(WHOLE); speaker_stop(); wait_ms(LONG_PAUSE);
    
    speaker_stop();
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