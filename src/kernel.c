#include "gdt.h"
#include "interrupts/idt.h"
#include "interrupts/interrupts.h"
#include "vga.h"
#include "stdint.h"
#include "debug_tools.h"

void kmain(void) {
    print("Welcome to Zephyrus!\n");
    newLine();

    initGdt();

    print_segment_registers();

    initIdt();
    enableInterrupts();

    print_current_cpu_state();
}