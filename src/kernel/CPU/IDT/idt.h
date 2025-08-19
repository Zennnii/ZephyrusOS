#ifndef IDT_H
#define IDT_H

#include "stdint.h"
#include "logf/logf.h"
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

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

#define PIC_EOI      0x20

#endif