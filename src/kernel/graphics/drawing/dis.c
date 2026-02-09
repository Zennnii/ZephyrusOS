#include "gfx.h"

void draw_pixel(int x, int y, uint32_t color) {
    if (x < 0 || y < 0 || x >= fb_width || y >= fb_height)
        return; // avoid drawing out of bounds
    fb[y * fb_width + x] = color;
}