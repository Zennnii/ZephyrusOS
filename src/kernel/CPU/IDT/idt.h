#ifndef IDT_H
#define IDT_H

#include "stdint.h"
#include "logf/logf.h"
#include "interrupts.h"

#define PIC1_COMMAND    0x20
#define PIC2_COMMAND    0xA0

#define PIC1_DATA       0x21
#define PIC2_DATA       0xA1

#define PIC_EOI         0x20

// Define the IDT entry structure
struct idt_entry_struct {
    uint16_t offset_low;   // Lower 16 bits of offset
    uint16_t selector;     // Kernel segment selector
    uint8_t alwaysZero;    // Always zero
    uint8_t flags;         // Flags
    uint16_t offset_high;  // Upper 16 bits of offset
} __attribute__((packed));

// Define the IDT pointer structure
struct idt_ptr_struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Function declarations
void initIdt();
void setIdtGate(uint8_t num, uint32_t offset, uint16_t sel, uint8_t flags);

#endif