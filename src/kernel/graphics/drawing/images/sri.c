#include "gfx.h"
#include "stdint.h"
#include "init.h"

struct sri_header {
    char magic[4];
    uint32_t _sriWidth;
    uint32_t _sriHeight;
};

bool parse_sri_header(uint8_t *data, struct sri_header *hdr) {
    // Check header
    if (data[0] != 'S' || data[1] != 'R' || data[2] != 'I' || data[3] != '2') {
        return false; // invalid file
    }

    hdr->magic[0] = data[0];
    hdr->magic[1] = data[1];
    hdr->magic[2] = data[2];
    hdr->magic[3] = data[3];

    hdr->_sriWidth = *(uint32_t*)(data + 4);
    hdr->_sriHeight = *(uint32_t*)(data + 8);

    return true;
}

// Draw an SRI image at (x0, y0), preserving RGB and replacing alpha with rgb_replace_alpha
void draw_sri(uint8_t *data, uint32_t x_pos, uint32_t y_pos, uint32_t rgb_replace_alpha) {
    struct sri_header hdr;
    if (!parse_sri_header(data, &hdr)) return;

    uint32_t *pixels_ptr = fb;
    uint32_t pitch_pixels = fb_pitch / 4;
    uint8_t *pixel_data = data + 16; // header = 16 bytes (no bpp field)

    for (uint32_t y = 0; y < hdr._sriHeight && (y + y_pos) < fb_height; y++) {
        for (uint32_t x = 0; x < hdr._sriWidth && (x + x_pos) < fb_width; x++) {
            uint32_t *src_pixel = (uint32_t*)(pixel_data + (y * hdr._sriWidth + x) * 4);
            uint8_t a = (*src_pixel >> 24) & 0xFF;
            uint8_t b = (*src_pixel >> 16) & 0xFF; // swapped
            uint8_t g = (*src_pixel >> 8) & 0xFF;
            uint8_t r = (*src_pixel >> 0) & 0xFF; // swapped

            uint32_t final_pixel;

            if (a == 0) {
                final_pixel = rgb_replace_alpha;
            } else {
                final_pixel = (0xFF << 24) | (r << 16) | (g << 8) | b;
            }

            pixels_ptr[(y + y_pos) * pitch_pixels + (x + x_pos)] = final_pixel;
        }
    }
}