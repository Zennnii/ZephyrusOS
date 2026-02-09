#ifndef GFX_H
#define GFX_H

#include "stdint.h"

// Character width + height definitions
#define CHAR_W 8
#define CHAR_H 8



// Extern declarations for current line and X + cursor logic
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



// --Text drawing + cursor logic functions--
extern void draw_char(uint32_t *fb, int fb_width, int x, int y, char c, uint32_t color);
extern void draw_string(uint32_t *fb, int fb_width, int x, int y, const char *s, uint32_t color);
extern void newLineFB();
extern void draw_dec(unsigned int num);
extern void draw_hex(uint32_t val);
extern void backspaceFB(uint32_t *fb, int fb_width, uint32_t bg);
extern void draw_cursor(uint32_t *fb, int fb_width, uint32_t color);
extern void erase_cursor(uint32_t *fb, int fb_width, uint32_t bg_color);
//--



// Clear FB function
extern void clear(uint32_t color);

// Scroll down function
extern void scroll_down(uint32_t bg_color);

// Draw pixel function
extern void draw_pixel(int x, int y, uint32_t color);



// Color extern declarations
extern const uint32_t colorWhite;
extern const uint32_t colorRed;
extern const uint32_t colorOrange;
extern const uint32_t colorBrown;
extern const uint32_t colorYellow;
extern const uint32_t colorGreen;
extern const uint32_t colorCyan;
extern const uint32_t colorBlue;
extern const uint32_t colorSkyBlue;
extern const uint32_t colorPurple;
extern const uint32_t colorPink;
extern const uint32_t colorHotPink;
extern const uint32_t colorBlack;



// Draw line function
extern void draw_line(int x0, int y0, int x1, int y1, uint32_t color);



// Draw rectangle functions (outline + filled)
extern void draw_rect(int x, int y, int w, int h, uint32_t color);
extern void draw_rect_filled(int x, int y, int w, int h, uint32_t color);

// Draw triangle functions (outline + filled)
extern void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

// Draw an SRI image at (x0, y0), preserving RGB and replacing alpha with rgb_replace_alpha
extern void draw_sri(uint8_t *data, uint32_t x0, uint32_t y0, uint32_t rgb_replace_alpha);

#endif