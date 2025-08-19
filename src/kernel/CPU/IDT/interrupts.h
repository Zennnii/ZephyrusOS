#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "stdint.h"

struct InterruptRegisters {
    // Pushed by pusha
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    // Pushed manually
    uint32_t ds;
    uint32_t int_no;
    uint32_t err_code;       // CPU pushed for some exceptions, else dummy 0
    // Pushed automatically by CPU
    uint32_t eip, cs, eflags, useresp, ss;
    uint32_t cr2;            // Only valid for Page Faults (#14)
};

#endif