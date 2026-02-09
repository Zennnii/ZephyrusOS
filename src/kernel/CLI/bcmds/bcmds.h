#ifndef BCMDS_H
#define BCDMS_H

#include "string.h"
#include "stdint.h"

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