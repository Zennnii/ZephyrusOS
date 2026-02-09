#ifndef COM_1
#define COM_1

#include "stdint.h"
#include "util/util.h"

#define COM1 0x3F8

void serial_init();
int serial_is_transmit_empty();
void serial_write_char(char c);
void serial_write(const char* s);
void serial_write_int(int x);
void serial_write_hex(unsigned int x);

#endif