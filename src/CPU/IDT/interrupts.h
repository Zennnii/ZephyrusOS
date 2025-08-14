#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "../../libs/stdint.h"

struct InterruptRegisters {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
    uint32_t cr2;
};

#endif