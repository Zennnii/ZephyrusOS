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
    mbi = (multiboot_info_t*)multiboot_info_addr;

    // Initialize GDT
    initGdt();

    // Initialize IDT
    initIdt();
    // Install ISR
    isr_install();
    // Install IRQ
    irq_install();
    // Enables interrupts (sti)
    enableInterrupts();
    
    // Initialize kernel heap (1.5mb)
    heap_init();

    // Set PIT Timers frequency (100 ticks a second)
    pit_set_frequency(100);

    // Initialize keyboard driver
    init_keyboard();

    // Initialize CMOS RTC driver
    init_rtc();

    LOG_INFO("Loading kernel CLI");

    wait(1);
    Reset();

    // Print welcome messsage
    print("Welcome to "); printcol("Zephyrus", COLOR8_LIGHT_BLUE); print("!\n");
    newLine();

    // Runs the kernels CLI
    cli();

    // Halts until next interrupt arrives
    while (1) {
        asm volatile ("hlt");
    }
     
}