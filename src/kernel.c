#include "CPU/GDT/gdt.h"
#include "CPU/IDT/idt.h"
#include "CPU/IDT/interrupts.h"
#include "CPU/IDT/IRQ/irq.h"
#include "CPU/IDT/isr.h"
#include "drivers/PIT/pit.h"
#include "drivers/PS2_Keyboard_Driver/keyboard.h"
#include "vga.h"
#include "stdint.h"
#include "debug_tools.h"

extern volatile uint32_t timer_ticks;
extern volatile uint32_t sys_uptime;

void kmain(void) {
    pit_set_frequency(100);

    print("Welcome to Zephyrus!\n");
    newLine();

    initGdt();

    print_segment_registers();

    initIdt();
    isr_install();
    irq_install();
    enableInterrupts();

    print_current_cpu_state();

    // Halts until next interrupt arrives
    while (1) {
        asm volatile ("hlt");
    }
     
}