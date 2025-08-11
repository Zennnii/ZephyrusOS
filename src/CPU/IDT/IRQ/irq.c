#include "irq.h"
#include "../idt.h"
#include "../../../util.h"
#include "../../../drivers/PIT/pit.h"
#include "../../../drivers/PS2_Keyboard_Driver/keyboard.h"

#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC_EOI 0x20

volatile uint32_t timer_ticks = 0;
volatile uint32_t sys_uptime = 0;

void send_eoi(uint8_t irq) {
    if (irq >= 8)
        outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

void irq_install() {

    // Remap PIC
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);

    outb(PIC1_DATA, 0x20);  // Master PIC vector offset
    outb(PIC2_DATA, 0x28);  // Slave PIC vector offset

    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);

    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    outb(PIC1_DATA, 0x0);
    outb(PIC2_DATA, 0x0);

    setIdtGate(32, (uint32_t)irq0, 0x08, 0x8E);   // Timer
    setIdtGate(33, (uint32_t)irq1, 0x08, 0x8E);   // Keyboard
    //setIdtGate(34, (uint32_t)irq2, 0x08, 0x8E);   // Cascade
    //setIdtGate(35, (uint32_t)irq3, 0x08, 0x8E);   // COM2/4
    //setIdtGate(36, (uint32_t)irq4, 0x08, 0x8E);   // COM1/3
    //setIdtGate(37, (uint32_t)irq5, 0x08, 0x8E);   // LPT2
    //setIdtGate(38, (uint32_t)irq6, 0x08, 0x8E);   // Floppy
    //setIdtGate(39, (uint32_t)irq7, 0x08, 0x8E);   // LPT1
    //setIdtGate(40, (uint32_t)irq8, 0x08, 0x8E);   // CMOS RTC
    //setIdtGate(41, (uint32_t)irq9, 0x08, 0x8E);   // Free
    //setIdtGate(42, (uint32_t)irq10, 0x08, 0x8E);  // Free
    //setIdtGate(43, (uint32_t)irq11, 0x08, 0x8E);  // Free
    //setIdtGate(44, (uint32_t)irq12, 0x08, 0x8E);  // PS/2 Mouse
    //setIdtGate(45, (uint32_t)irq13, 0x08, 0x8E);  // FPU
    //setIdtGate(46, (uint32_t)irq14, 0x08, 0x8E);  // Primary ATA
    //setIdtGate(47, (uint32_t)irq15, 0x08, 0x8E);  // Secondary ATA
}

void irq0_handler(struct interruptRegisters *regs) {
    pit_set_frequency(100);

    timer_ticks++;

    // increases system uptime every 100 ticks which is every seconds
    if (timer_ticks % 100 == 0) {
        sys_uptime++;
    }

    send_eoi(0);
}

void irq1_handler() {
    keyboard_isr_handler();
}