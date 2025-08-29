#ifndef LOGF_H
#define LOGF_H

#include "vga.h"
#include "stdint.h"
#include "drivers/cmos_rtc/cmos_rtc.h"

// Colors
#define LOG_COLOR_INFO  COLOR8_LIGHT_GREEN
#define LOG_COLOR_WARN  COLOR8_LIGHT_RED
#define LOG_COLOR_ERR   COLOR8_RED
#define LOG_COLOR_LOAD COLOR8_LIGHT_BROWN

// Logger
void klog(const char* level, char* msg, uint16_t color, uint8_t nlflag);

#define LOG_INFO(msg)  klog("INFO", msg, LOG_COLOR_INFO, 1)
#define LOG_WARN(msg)  klog("WARN", msg, LOG_COLOR_WARN, 1)
#define LOG_ERR(msg)   klog("ERR", msg, LOG_COLOR_ERR, 1)
#define LOG_LOAD(msg) klog("LOAD", msg, LOG_COLOR_LOAD, 1)

#define LOG_INFO_NONL(msg)  klog("INFO", msg, LOG_COLOR_INFO, 0)
#define LOG_WARN_NONL(msg)  klog("WARN", msg, LOG_COLOR_WARN, 0)
#define LOG_ERR_NONL(msg)   klog("ERR", msg, LOG_COLOR_ERR, 0)
#define LOG_LOAD_NONL(msg) klog("LOAD", msg, LOG_COLOR_LOAD, 0)

#endif