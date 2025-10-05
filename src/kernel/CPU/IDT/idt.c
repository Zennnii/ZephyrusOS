#include "idt.h"
#include "util/util.h"

extern void idt_flush(uint32_t);

volatile bool idt_loaded = false;

struct idt_entry_struct idt_entries[256];
struct idt_ptr_struct idt_ptr;

// Intialize IDT function
void initIdt() {
    idt_ptr.limit = sizeof(struct idt_entry_struct) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    memset(&idt_entries, 0, sizeof(struct idt_entry_struct) * 256);

    idt_flush((uint32_t)&idt_ptr);

    idt_loaded = true;
}

// Set IDT gate function
void setIdtGate(uint8_t num, uint32_t offset, uint16_t sel, uint8_t flags) {
    idt_entries[num].offset_low = offset & 0xFFFF;
    idt_entries[num].offset_high = (offset >> 16) & 0xFFFF;
    idt_entries[num].selector = sel;
    idt_entries[num].alwaysZero = 0;
    idt_entries[num].flags = flags;
}

void confirmIdtLoaded() {
    if (idt_loaded == true) {
    LOG_INFO("IDT Loaded");
    }
}