#ifndef PANIC_H
#define PANIC_H

#include "stdint.h"
#include "fb/dis.h"
#include "fb/colors.h"
#include "util/util.h"

void kernelPanic(const char* errorMessage, uint32_t errorCode);

#endif