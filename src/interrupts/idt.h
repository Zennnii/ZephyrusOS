#define __STDINT_H
#ifndef IDT_H
#define IDT_H

#include "../stdint.h"
#include "interrupts.h"

struct idt_entry_struct {
    uint16_t offset_low; // Lower 16 bits of handler address
    uint16_t selector; // Kernel code segment selector in GDT
    uint8_t alwaysZero; // Always zero
    uint8_t flags; // Flags (gate type, dpl, and p fields)
    uint16_t offset_high; // Upper 16 bits of handler address
} __attribute__((packed));

struct idt_ptr_struct {
    uint16_t limit; // Size of the IDT (bytes - 1)
    uint32_t base; // Address of the first IDT entry
} __attribute__((packed));

void initIdt();
void setIdtGate(uint8_t num, uint32_t offset, uint16_t sel, uint8_t flags);

void isr_handler(struct InterruptRegisters* regs);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void isr128();
extern void isr177();

extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

#endif