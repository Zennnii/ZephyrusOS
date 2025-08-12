#include "../../stdint.h"

#define KERNEL_HEAP_START 0x01000000
#define KERNEL_HEAP_SIZE  0x00180000  // 1.5 MB
#define KERNEL_HEAP_END   (KERNEL_HEAP_START + KERNEL_HEAP_SIZE)

static uintptr_t heap_start;
static uintptr_t heap_end;

void* kmalloc(size_t size);

void kfree(void* ptr);