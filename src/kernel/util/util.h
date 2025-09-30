#ifndef UTIL_H
#define UTIL_H

#include "stdint.h"
#include "CPU/IDT/IRQ/irq.h"
#include "drivers/PIT/pit.h"

// Memset function
void memset(void *dest, char val, uint32_t count);

void *memcpy(void *dest, const void *src, size_t n);

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

void wait_ms(uint32_t ms);

int atoi(const char *s);

uint16_t inw(uint16_t port);

void outw(uint16_t port, uint16_t val);

void insw(uint16_t port, void *addr, int count);

void outsw(uint16_t port, const void *addr, int count);

#endif