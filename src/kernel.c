#include "vga.h"
#include "gdt.h"
#include "stdint.h"

void print_segment_registers() {
    uint16_t cs, ds, es, fs, gs, ss;

    asm volatile ("mov %%cs, %0" : "=r"(cs));
    asm volatile ("mov %%ds, %0" : "=r"(ds));
    asm volatile ("mov %%es, %0" : "=r"(es));
    asm volatile ("mov %%fs, %0" : "=r"(fs));
    asm volatile ("mov %%gs, %0" : "=r"(gs));
    asm volatile ("mov %%ss, %0" : "=r"(ss));

    print("Segment registers:\n");
    print("CS = 0x"); printHex(cs); print("\n");
    print("DS = 0x"); printHex(ds); print("\n");
    print("ES = 0x"); printHex(es); print("\n");
    print("FS = 0x"); printHex(fs); print("\n");
    print("GS = 0x"); printHex(gs); print("\n");
    print("SS = 0x"); printHex(ss); print("\n");
}

void kmain(void) {
    print("Welcome to Zephyrus!\n");
    newLine();

    initGdt();

    print("GDT has been initialized successfully\n");
    newLine();

    print_segment_registers();
}