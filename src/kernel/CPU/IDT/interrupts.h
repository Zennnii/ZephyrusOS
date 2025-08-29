#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "stdint.h"

struct InterruptRegisters {
    uint32_t ds;       // pushed manually in stub
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp_dummy; // original esp pushed by pusha
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t int_no;   // pushed in stub
    uint32_t err_code; // pushed by CPU or dummy
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t useresp;
    uint32_t ss;
};

#endif
