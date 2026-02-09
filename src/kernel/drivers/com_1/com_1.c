#include "com_1.h"

void serial_init() {
    outb(COM1 + 1, 0x00);    // Disable interrupts
    outb(COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(COM1 + 0, 0x03);    // Divisor low byte  (38400 baud)
    outb(COM1 + 1, 0x00);    // Divisor high byte
    outb(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7);    // Enable FIFO, clear, 14-byte threshold
    outb(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int serial_is_transmit_empty() {
    return inb(0x3F8 + 5) & 0x20;
}

void serial_write_char(char c) {
    while (!serial_is_transmit_empty());
    outb(COM1, c);
}

void serial_write(const char* str) {
    while (*str) {
        serial_write_char(*str++);
    }
}

void serial_write_int(int x) {
    if (x == 0) {
        serial_write("0");
        return;
    }

    char buf[16];
    int i = 0;

    int neg = 0;
    if (x < 0) {
        neg = 1;
        x = -x;
    }

    while (x > 0) {
        buf[i++] = '0' + (x % 10);
        x /= 10;
    }

    if (neg)
        buf[i++] = '-';

    buf[i] = 0;

    // reverse the buffer
    for (int j = i - 1; j >= 0; j--)
        serial_write_char(buf[j]);
}

void serial_write_hex(unsigned int x) {
    const char *hex = "0123456789ABCDEF";
    serial_write("0x");

    for (int i = 28; i >= 0; i -= 4) {
        serial_write_char(hex[(x >> i) & 0xF]);
    }
}