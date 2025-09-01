#include "util.h"

extern volatile uint32_t timer_ticks;

void memset(void *dest, char val, uint32_t count) {
    char *temp = (char*) dest;
    for (; count != 0; count --) {
        *temp++ = val;
    }
}

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;

    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}


void enableInterrupts() {
    asm volatile("sti");
}

void disableInterrupts() {
    asm volatile("cli");
}

void wait(uint32_t sec) {
    uint32_t target_ticks = timer_ticks + (sec * 100); // 100 ticks per second
    
    while (timer_ticks < target_ticks) {
        asm volatile("hlt"); // Halt CPU until next interrupt
    }
}

void wait_ms(uint32_t ms) {
    uint32_t target_ticks = timer_ticks + ((ms * 100) / 1000); // 100 ticks/sec
    while (timer_ticks < target_ticks) {
        asm volatile("hlt");
    }
}

int atoi(const char *s) {
    int res = 0;
    while (*s) {
        res = res * 10 + (*s - '0');
        s++;
    }
    return res;
}


uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

void insw(uint16_t port, void *addr, int count) {
    __asm__ volatile (
        "push %%ds\n\t"
        "mov %%es, %%ax\n\t"
        "mov %%ax, %%ds\n\t"
        "cld\n\t"
        "rep insw\n\t"
        "pop %%ds"
        :
        : "D"(addr), "c"(count), "d"(port)
        : "ax", "memory"
    );
}

// --- String output (write multiple words) ---
void outsw(uint16_t port, const void *addr, int count) {
    __asm__ volatile (
        "cld\n\t"
        "rep outsw"
        : "+S"(addr), "+c"(count)
        : "d"(port)
    );
}
