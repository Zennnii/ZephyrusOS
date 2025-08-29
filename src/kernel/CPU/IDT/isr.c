#include "isr.h"

volatile uint32_t exception_occurred = 0;
volatile uint32_t last_exception_code = 0;
volatile uint32_t last_error_code = 0;

void (*isr_array[32])() = {
    isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7,
    isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15,
    isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23,
    isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31
};

void isr_install() {
    for (uint8_t i = 0; i < 32; i++)
        setIdtGate(i, (uint32_t)isr_array[i], 0x08, 0x8E);

    setIdtGate(128, (uint32_t)isr128, 0x08, 0x8E);
    setIdtGate(177, (uint32_t)isr177, 0x08, 0x8E);
}

void isr_handler(struct InterruptRegisters* regs) {
    if (regs->int_no >= 32 && regs->int_no < 48) {
        if (regs->int_no >= 40) outb(PIC2_COMMAND, PIC_EOI);
        outb(PIC1_COMMAND, PIC_EOI);
    }

    switch (regs->int_no) {
        case 0: // Divide by zero
            exception_occurred = 1;
            last_exception_code = 0;
            last_error_code = regs->err_code;
            while(1) { __asm__ volatile("cli; hlt"); }
            break;
        case 13: // GPF
            exception_occurred = 1;
            last_exception_code = 13;
            last_error_code = regs->err_code;
            break;
        case 14: // Page Fault
            exception_occurred = 1;
            last_exception_code = 14;
            uint32_t fault_addr;
            asm volatile("mov %%cr2, %0" : "=r"(fault_addr));
            last_error_code = fault_addr;
            break;
        case 32: irq0_handler(); break;
        case 33: keyboard_isr_handler(); break;
        case 40: rtc_isr_handler(); break;
        default:
            if (regs->int_no < 32) {
                exception_occurred = 1;
                last_exception_code = regs->int_no;
                last_error_code = regs->err_code;
            }
            break;
    }
}

void check_exceptions() {
    if (!exception_occurred) return;

    switch (last_exception_code) {
        case 0: kernelPanic("Divide By Zero Exception", last_error_code); break;
        case 13: kernelPanic("General Protection Fault", last_error_code); break;
        case 14: kernelPanic("Page Fault", last_error_code); break;
        default: kernelPanic("Unknown Exception", last_exception_code); break;
    }
}
