#ifndef BCMDS_H
#define BCDMS_H

#include "../../libs/string.h"
#include "../../libs/stdint.h"
#include "../../vga.h"
#include "../shell.h"
#include "../../util.h"

#define LINE_BUFFER_SIZE 128

extern char echoIn[LINE_BUFFER_SIZE];
void echof();
void clearf();
void verf();
void helpf();
void shutdownf();

#endif