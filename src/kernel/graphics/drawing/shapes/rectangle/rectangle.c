#include "gfx.h"

// Draw a horizontal line
void draw_hline(int x, int y, int length, uint32_t color) {
    for (int i = 0; i < length; i++) {
        draw_pixel(x + i, y, color);
    }
}

// Draw a vertical line
void draw_vline(int x, int y, int length, uint32_t color) {
    for (int i = 0; i < length; i++) {
        draw_pixel(x, y + i, color);
    }
}

// Draw rectangle (outline)
void draw_rect(int x, int y, int w, int h, uint32_t color) {
    // Args: X start, Y start, Width, Height, Color
    draw_hline(x, y, w, color);           // top
    draw_hline(x, y + h - 1, w, color);   // bottom
    draw_vline(x, y, h, color);           // left
    draw_vline(x + w - 1, y, h, color);   // right
}

// Draw rectangle (filled)
void draw_rect_filled(int x, int y, int w, int h, uint32_t color) {
    // Args: X start, Y start, Width, Height, Color
    for (int row = 0; row < h; row++) {
        draw_hline(x, y + row, w, color);
    }
}