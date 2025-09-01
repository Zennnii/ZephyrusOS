#ifndef ISR_H
#define ISR_H

#include "idt.h"
#include "interrupts.h"
#include "vga.h"
#include "util.h"
#include "drivers/PIT/pit.h"
#include "drivers/PS2_Keyboard_Driver/keyboard.h"
#include "drivers/cmos_rtc/cmos_rtc.h"
#include "drivers/ata/ata.h"

// ISR array
extern void (*isr_array[32])();

// ISR installation and handler
void isr_install();
void isr_handler(struct InterruptRegisters* regs);
void check_exceptions();

// ISR extern declarations (0-31 exceptions)
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

// System calls
extern void isr128();
extern void isr177();

// IRQ handlers
void irq0_handler();
void rtc_isr_handler();
void keyboard_isr_handler();

#endif
