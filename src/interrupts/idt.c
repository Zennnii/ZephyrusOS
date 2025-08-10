// idt.c
#include "idt.h"
#include "../util.h"
#include "../vga.h"

struct idt_entry_struct idt_entries[256];
struct idt_ptr_struct idt_ptr;

extern void idt_flush(uint32_t);

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

#define PIC_EOI      0x20

void initIdt() {
    idt_ptr.limit = sizeof(struct idt_entry_struct) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    memset(&idt_entries, 0, sizeof(struct idt_entry_struct) * 256);

    // Remap PIC
    outPortB(PIC1_COMMAND, 0x11);
    outPortB(PIC2_COMMAND, 0x11);

    outPortB(PIC1_DATA, 0x20);  // Master PIC vector offset
    outPortB(PIC2_DATA, 0x28);  // Slave PIC vector offset

    outPortB(PIC1_DATA, 0x04);
    outPortB(PIC2_DATA, 0x02);

    outPortB(PIC1_DATA, 0x01);
    outPortB(PIC2_DATA, 0x01);

    outPortB(PIC1_DATA, 0x0);
    outPortB(PIC2_DATA, 0x0);

    // Set ISR gates for exceptions (0-31)
    setIdtGate(0, (uint32_t)isr0, 0x08, 0x8E);
    setIdtGate(1, (uint32_t)isr1, 0x08, 0x8E);
    setIdtGate(2, (uint32_t)isr2, 0x08, 0x8E);
    setIdtGate(3, (uint32_t)isr3, 0x08, 0x8E);
    setIdtGate(4, (uint32_t)isr4, 0x08, 0x8E);
    setIdtGate(5, (uint32_t)isr5, 0x08, 0x8E);
    setIdtGate(6, (uint32_t)isr6, 0x08, 0x8E);
    setIdtGate(7, (uint32_t)isr7, 0x08, 0x8E);
    setIdtGate(8, (uint32_t)isr8, 0x08, 0x8E);
    setIdtGate(9, (uint32_t)isr9, 0x08, 0x8E);
    setIdtGate(10, (uint32_t)isr10, 0x08, 0x8E);
    setIdtGate(11, (uint32_t)isr11, 0x08, 0x8E);
    setIdtGate(12, (uint32_t)isr12, 0x08, 0x8E);
    setIdtGate(13, (uint32_t)isr13, 0x08, 0x8E);
    setIdtGate(14, (uint32_t)isr14, 0x08, 0x8E);
    setIdtGate(15, (uint32_t)isr15, 0x08, 0x8E);
    setIdtGate(16, (uint32_t)isr16, 0x08, 0x8E);
    setIdtGate(17, (uint32_t)isr17, 0x08, 0x8E);
    setIdtGate(18, (uint32_t)isr18, 0x08, 0x8E);
    setIdtGate(19, (uint32_t)isr19, 0x08, 0x8E);
    setIdtGate(20, (uint32_t)isr20, 0x08, 0x8E);
    setIdtGate(21, (uint32_t)isr21, 0x08, 0x8E);
    setIdtGate(22, (uint32_t)isr22, 0x08, 0x8E);
    setIdtGate(23, (uint32_t)isr23, 0x08, 0x8E);
    setIdtGate(24, (uint32_t)isr24, 0x08, 0x8E);
    setIdtGate(25, (uint32_t)isr25, 0x08, 0x8E);
    setIdtGate(26, (uint32_t)isr26, 0x08, 0x8E);
    setIdtGate(27, (uint32_t)isr27, 0x08, 0x8E);
    setIdtGate(28, (uint32_t)isr28, 0x08, 0x8E);
    setIdtGate(29, (uint32_t)isr29, 0x08, 0x8E);
    setIdtGate(30, (uint32_t)isr30, 0x08, 0x8E);
    setIdtGate(31, (uint32_t)isr31, 0x08, 0x8E);

    setIdtGate(32, (uint32_t)irq0, 0x08, 0x8E);   // Timer
    setIdtGate(33, (uint32_t)irq1, 0x08, 0x8E);   // Keyboard
    setIdtGate(34, (uint32_t)irq2, 0x08, 0x8E);   // Cascade
    setIdtGate(35, (uint32_t)irq3, 0x08, 0x8E);   // COM2/4
    setIdtGate(36, (uint32_t)irq4, 0x08, 0x8E);   // COM1/3
    setIdtGate(37, (uint32_t)irq5, 0x08, 0x8E);   // LPT2
    setIdtGate(38, (uint32_t)irq6, 0x08, 0x8E);   // Floppy
    setIdtGate(39, (uint32_t)irq7, 0x08, 0x8E);   // LPT1
    setIdtGate(40, (uint32_t)irq8, 0x08, 0x8E);   // CMOS RTC
    setIdtGate(41, (uint32_t)irq9, 0x08, 0x8E);   // Free
    setIdtGate(42, (uint32_t)irq10, 0x08, 0x8E);  // Free
    setIdtGate(43, (uint32_t)irq11, 0x08, 0x8E);  // Free
    setIdtGate(44, (uint32_t)irq12, 0x08, 0x8E);  // PS/2 Mouse
    setIdtGate(45, (uint32_t)irq13, 0x08, 0x8E);  // FPU
    setIdtGate(46, (uint32_t)irq14, 0x08, 0x8E);  // Primary ATA
    setIdtGate(47, (uint32_t)irq15, 0x08, 0x8E);  // Secondary ATA

    // System calls or custom interrupts
    setIdtGate(128, (uint32_t)isr128, 0x08, 0x8E);
    setIdtGate(177, (uint32_t)isr177, 0x08, 0x8E);

    idt_flush((uint32_t)&idt_ptr);

    print("IDT has been initialized successfully\n");
    newLine();
}

void setIdtGate(uint8_t num, uint32_t offset, uint16_t sel, uint8_t flags) {
    idt_entries[num].offset_low = offset & 0xFFFF;
    idt_entries[num].offset_high = (offset >> 16) & 0xFFFF;
    idt_entries[num].selector = sel;
    idt_entries[num].alwaysZero = 0;
    idt_entries[num].flags = flags;
}

void isr_handler(struct InterruptRegisters* regs) {

    switch (regs->int_no) {
        case 0:
            print("Exception: Divide By Zero\n");
            newLine();
            break;
        case 13:
            print("Exception: General Protection Fault\n");
            newLine();
            break;
        case 14:
            print("Exception: Page Fault\n");
            print("Faulting address: ");
            print_hex(regs->cr2);
            print("\n");
            break;
        case 32:
            // Timer IRQ
            break;
        case 33:
            // Keyboard IRQ
            break;
        default:
            print("Unhandled interrupt\n");
            break;
    }

    if (regs->int_no >= 40) { // Slave PIC
        outPortB(PIC2_COMMAND, PIC_EOI);
    }
    if (regs->int_no >= 32) { // Master PIC
        outPortB(PIC1_COMMAND, PIC_EOI);
    }
}

void enableInterrupts() {
    asm volatile("sti");
}