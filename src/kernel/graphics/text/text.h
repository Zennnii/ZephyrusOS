#ifndef TEXT_H
#define TEXT_H

#include "stdint.h"

// Character width + height definitions
#define CHAR_W 8
#define CHAR_H 8

extern uint8_t font8x8_basic[128][8];
extern int curLine;
extern int curX;

extern int cursor_x;
extern int cursor_y;

#endif