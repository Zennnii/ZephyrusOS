#ifndef BCMDS_H
#define BCDMS_H

#include "string.h"
#include "stdint.h"
#include "vga.h"
#include "../cli.h"
#include "util/util.h"
#include "drivers/PIT/pit.h"
#include "drivers/cmos_rtc/cmos_rtc.h"
#include "../bmcli/bmcli.h"
#include "drivers/Speaker/speaker.h"
#include "fs/fat16/fat16.h"
#include "fb/dis.h"
#include "fb/colors.h"
#include "panic/panic.h"
#include "debug/debug_tools.h"

#define LINE_BUFFER_SIZE 256

extern char echoIn[LINE_BUFFER_SIZE];
void echof();
void clearf();
void verf();
void helpf();
void shutdownf();
void reboot8042();
void uptimef();
void panicf();
void colorsf();
void timef();
void beepf(int argc, char *argv[]);
void rdf(int argc, char **argv);
void regdumpf();
void musicf();
void exitf();

#endif