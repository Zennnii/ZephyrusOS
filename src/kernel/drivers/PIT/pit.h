#ifndef PIT_H
#define PIT_H

#include "stdint.h"
#include "util.h"
#include "logf/logf.h"

void pit_set_frequency(uint32_t hz);

extern volatile uint32_t timer_ticks;
extern volatile uint32_t sys_uptime;

#endif