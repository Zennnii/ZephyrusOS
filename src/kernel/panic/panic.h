#ifndef PANIC_H
#define PANIC_H

#include "stdint.h"

void panic(const char* errorMessage, uint32_t errorCode);

#endif