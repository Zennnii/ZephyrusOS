#ifndef SHELL_H
#define SHELL_H

#include "../drivers/PS2_Keyboard_Driver/keyboard.h"
#include "../libs/stdint.h"
#include "../libs/string.h"
#include "../vga.h"
#include "bcmds/bcmds.h"

#define LINE_BUFFER_SIZE 128
#define MAX_ARGS 10
#define MAX_INPUT 128
extern char userIn[LINE_BUFFER_SIZE];

extern int argc;
extern char *argv[MAX_ARGS];

void parse_args(char *input);
void execute_command(char *userIn);
void shell();
void getline(char *out);

#endif