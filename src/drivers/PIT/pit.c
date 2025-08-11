#include "pit.h"

/* 
0x43 = PIT command register
0x36 = 00110110 Select channel 0, Acces mode (low byte then high byte), Operating mode 3 (square wave generator), Binary counting mode
outb(0x40, divisor & 0xFF); = Sends low 8 bits of divisor
outb(0x40, divisor >> 8); = Send high 8 bits of divisor
*/

void pit_set_frequency(uint32_t hz) {
    uint32_t divisor = 1193182 / hz; // PIT Clock / Desired frequency
    outb(0x43, 0x36); // Command: channel 0, lobyte/hibyte, mode 3
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
}