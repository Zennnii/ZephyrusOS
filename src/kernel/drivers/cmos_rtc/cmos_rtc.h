#ifndef CMOS_RTC_H
#define CMOS_RTC_H

#include "stdint.h"
#include "util/util.h"
#include "vga.h"
#include "logf/logf.h"
#include "fb/dis.h"
#include "fb/colors.h"

static inline uint8_t cmos_read(uint8_t reg);
static inline int rtc_update_in_progress(void);
static inline uint8_t bcd_to_bin(uint8_t val);
void print_date_time();
void print_time_log();
void rtc_isr_handler();
void init_rtc();

#endif