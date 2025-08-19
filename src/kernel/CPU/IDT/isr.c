#include "isr.h"

void isr_install() {
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

    // System calls
    setIdtGate(128, (uint32_t)isr128, 0x08, 0x8E);
    setIdtGate(177, (uint32_t)isr177, 0x08, 0x8E);
}

void isr_handler(struct InterruptRegisters* regs) {

    switch (regs->int_no) {
        case 0:
            print("Exception: Divide By Zero\n");
            newLine();
            while(1) { __asm__ volatile ("hlt"); }
            break;
        case 13: // General Protection Fault
            kernelPanic("General Protection Fault", regs->err_code);
            break;
        case 14: // Page Fault
            kernelPanic("Page Fault at address", regs->cr2);
            break;
        case 32:
            irq0_handler();
        case 33:
            keyboard_isr_handler();
            break;
        case 40:
            rtc_isr_handler();
        default:
            print("Unhandled interrupt\n");
            while(1) { __asm__ volatile ("hlt"); }
            break;
    }

    if (regs->int_no >= 40) { // Slave PIC
        outb(PIC2_COMMAND, PIC_EOI);
    }
    if (regs->int_no >= 32) { // Master PIC
        outb(PIC1_COMMAND, PIC_EOI);
    }
}
