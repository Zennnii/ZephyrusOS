#ifndef DEBUG_TOOLS_H
#define DEBUG_TOOLS_H

#include "stdint.h"
#include "CPU/IDT/idt.h"
#include "mm/kmalloc/kmalloc.h"
#include "gfx.h"

void print_current_cpu_state();
void print_segment_registers();
void test_kmalloc();
void heap_selftest(void);

#endif