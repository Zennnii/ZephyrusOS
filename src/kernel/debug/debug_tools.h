#ifndef DEBUG_TOOLS_H
#define DEBUG_TOOLS_H

#include "stdint.h"
#include "CPU/IDT/idt.h"
#include "mm/kmalloc/kmalloc.h"
#include "fb/dis.h"
#include "fb/colors.h"

void print_current_cpu_state();
void print_segment_registers();
void test_kmalloc();

#endif