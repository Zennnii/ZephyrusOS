#ifndef GDT_H
#define GDT_H

#include "stdint.h"
#include "logf/logf.h"
#include "CLI/bmcli/bmcli.h"

struct gdt_entry_struct {
    uint16_t limit;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t flags;
    uint8_t base_high;
} __attribute__((packed));

struct gdt_entry_ptr {
    uint16_t limit;
    unsigned int base;
} __attribute__((packed));

typedef uint32_t addr_t;

struct tss_entry_struct {
    uint32_t prev_tss;
    uint32_t esp0;
    uint32_t ss0;   uint16_t __res0;
    uint32_t esp1;
    uint32_t ss1;   uint16_t __res1;
    uint32_t esp2;
    uint32_t ss2;   uint16_t __res2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint16_t es;    uint16_t __res3;
    uint16_t cs;    uint16_t __res4;
    uint16_t ss;    uint16_t __res5;
    uint16_t ds;    uint16_t __res6;
    uint16_t fs;    uint16_t __res7;
    uint16_t gs;    uint16_t __res8;
    uint16_t ldt;   uint16_t __res9;
    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((packed));

void initGdt();
void setGdtGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
void enableInterrupts();
void confirmGdtLoaded();
void writeTSS(uint16_t num, uint16_t ss0, uint32_t esp0);

#endif