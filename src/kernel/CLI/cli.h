#ifndef CLI_H
#define CLI_H

#include "stdint.h"
#include "drivers/PS2_Keyboard_Driver/keyboard.h"
#include "string.h"
#include "vga.h"
#include "bcmds/bcmds.h"
#include "pcid_cmds/pcid.h"
#include "fb/dis.h"
#include "fb/colors.h"

#define LINE_BUFFER_SIZE 256
#define MAX_ARGS 30
#define MAX_INPUT 256
extern char userIn[LINE_BUFFER_SIZE];

extern int argc;
extern char *argv[MAX_ARGS];
extern bool cli_running;

void parse_args(char *input);
void execute_command(char *userIn);
void draw_prompt();
void cli();
void getline(char *out);

#endif