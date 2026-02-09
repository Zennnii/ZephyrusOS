#include "gfx.h"

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    /*    (x0, y0)
             /\
            /  \
           /    \
  (x1, y1)/______\(x2, y2)
 */
//draw_triangle(50, 50, 200, 80, 120, 200, 0xFFFFFFFF);
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}