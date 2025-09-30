#ifndef BMCLI_H
#define BMCLI_H

#include "stdint.h"
#include "drivers/PS2_Keyboard_Driver/keyboard.h"
#include "bmcmds/bmcmds.h"
#include "fb/dis.h"
#include "fb/colors.h"

extern bool bare_cli_running;

void execute_commandb(char *userIn);
void bare_cli();
void getline(char *out);

#endif