#include "CPU/GDT/gdt.h"
#include "CPU/IDT/idt.h"
#include "CPU/IDT/interrupts.h"
#include "CPU/IDT/IRQ/irq.h"
#include "CPU/IDT/isr.h"
#include "drivers/PIT/pit.h"
#include "drivers/PS2_Keyboard_Driver/keyboard.h"
#include "drivers/cmos_rtc/cmos_rtc.h"
#include "vga.h"
#include "stdint.h"
#include "debug_tools.h"
#include "CLI/cli.h"
#include "multiboot.h"

multiboot_info_t* mbi;

void kmain(uint32_t multiboot_info_addr) {
    // Ensure interrupts are disabled during critical setup
    __asm__ volatile("cli");
    
    mbi = (multiboot_info_t*)multiboot_info_addr;

    // Phase 1: Core CPU Setup (interrupts disabled)
    LOG_LOAD("Initializing GDT...");
    initGdt();
    
    LOG_LOAD("Initializing IDT...");
    initIdt();

    LOG_LOAD("Installing ISRs...");
    isr_install();
    
    LOG_LOAD("Installing IRQs...");
    irq_install();
    
    // Phase 2: Memory Management (still interrupts disabled)
    LOG_LOAD("Initializing Heap...");
    heap_init();
    
    // Phase 3: Enable interrupts and test
    LOG_LOAD("Enabling Interrupts...");
    enableInterrupts();
    
    // Small delay to test if interrupts work without crashing
    for (volatile int i = 0; i < 1000000; i++);
    
    // Phase 4: Initialize hardware drivers (interrupts now enabled)
    LOG_LOAD("Initializing PIT...");
    pit_set_frequency(100);
    
    LOG_LOAD("Initializing PS/2 Keyboard Driver...");
    init_keyboard();
    
    LOG_LOAD("Initializng CMOS RTC...");
    init_rtc();

    // Phase 5: System ready
    newLine();
    LOG_INFO("Kernel initialization complete");
    LOG_INFO("Loading kernel CLI...");

    // Brief pause before starting CLI
    wait(2);
    Reset();

    print("Welcome to "); 
    printcol("Zephyrus", COLOR8_LIGHT_BLUE); 
    print("!\n");
    newLine();

    // Phase 6: Main kernel loop
    cli();

    wait(2);

    while (1) {
        check_exceptions();
        __asm__ volatile("hlt"); 
    }
}