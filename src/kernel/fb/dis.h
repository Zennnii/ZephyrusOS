#ifndef DIS_H
#define DIS_H

#include "stdint.h"

#define CHAR_W 8
#define CHAR_H 8

extern uint8_t font8x8_basic[128][8];
extern int curLine;
extern int curX;

extern int cursor_x;
extern int cursor_y;

// Framebuffer globals
extern uint32_t *fb;
extern uint32_t fb_width;
extern uint32_t fb_height;
extern uint32_t fb_pitch;
extern uint32_t fb_pitch_pixels;
extern uint32_t *pixels;

void draw_char(uint32_t *fb, int fb_width, int x, int y, char c, uint32_t color);
void draw_string(uint32_t *fb, int fb_width, int x, int y, const char *s, uint32_t color);
void newLineFB();
void draw_dec(unsigned int num);
void draw_hex(uint32_t val);
void backspaceFB(uint32_t *fb, int fb_width, uint32_t bg);
void draw_cursor(uint32_t *fb, int fb_width, uint32_t color);
void erase_cursor(uint32_t *fb, int fb_width, uint32_t bg_color);
void clear(uint32_t color);
void scroll_down(uint32_t bg_color);

#endif