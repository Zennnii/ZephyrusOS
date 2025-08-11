#include "util.h"

extern volatile uint32_t timer_ticks;

void memset(void *dest, char val, uint32_t count) {
    char *temp = (char*) dest;
    for (; count != 0; count --) {
        *temp++ = val;
    }
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