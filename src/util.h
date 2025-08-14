#ifndef UTIL_H
#define UTIL_H

#include "libs/stdint.h"
#include "CPU/IDT/IRQ/irq.h"
#include "drivers/PIT/pit.h"

// Memset function
void memset(void *dest, char val, uint32_t count);

// Out port function
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "dN"(port));
}

// In port function
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}
void enableInterrupts();

void disableInterrupts();

void wait(uint32_t sec);

#endif