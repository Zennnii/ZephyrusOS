#ifndef LOGF_H
#define LOGF_H

#include "stdint.h"

// Colors
#define LOG_COLOR_INFO  0x00FF00    // Green
#define LOG_COLOR_WARN  0xFF7F00    // Orange  
#define LOG_COLOR_ERR   0xFF0000    // Red
#define LOG_COLOR_LOAD  0xFFFF00    // Yellow

// Logger
void klog(const char* level, char* msg, uint32_t color, bool nlflag);
void printKlog(const char* level, char* msg, uint32_t color, uint8_t nlflag);

#define LOG_INFO(msg)  klog("INFO", msg, LOG_COLOR_INFO, true)
#define LOG_WARN(msg)  klog("WARN", msg, LOG_COLOR_WARN, true)
#define LOG_ERR(msg)   klog("ERR", msg, LOG_COLOR_ERR, true)
#define LOG_LOAD(msg) klog("LOAD", msg, LOG_COLOR_LOAD, true)

#define LOG_INFO_NONL(msg)  klog("INFO", msg, LOG_COLOR_INFO, false)
#define LOG_WARN_NONL(msg)  klog("WARN", msg, LOG_COLOR_WARN, false)
#define LOG_ERR_NONL(msg)   klog("ERR", msg, LOG_COLOR_ERR, false)
#define LOG_LOAD_NONL(msg) klog("LOAD", msg, LOG_COLOR_LOAD, false)



#define PRINT_INFO(msg)  printKlog("INFO", msg, LOG_COLOR_INFO, true)
#define PRINT_WARN(msg)  printKlog("WARN", msg, LOG_COLOR_WARN, true)
#define PRINT_ERR(msg)   printKlog("ERR", msg, LOG_COLOR_ERR, true)
#define PRINT_LOAD(msg) printKlog("LOAD", msg, LOG_COLOR_LOAD, true)

#define PRINT_INFO_NONL(msg)  printKlog("INFO", msg, LOG_COLOR_INFO, false)
#define PRINT_WARN_NONL(msg)  printKlog("WARN", msg, LOG_COLOR_WARN, false)
#define PRINT_ERR_NONL(msg)   printKlog("ERR", msg, LOG_COLOR_ERR, false)
#define PRINT_LOAD_NONL(msg) printKlog("LOAD", msg, LOG_COLOR_LOAD, false)

#endif