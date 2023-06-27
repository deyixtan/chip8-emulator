#include "chip8display.h"

void chip8display_assert_position(uint8_t x, uint8_t y) {
    // don't need to check lower bound, since x and y are unsigned
    assert(x < CHIP8_DISPLAY_WIDTH);
    assert(y < CHIP8_DISPLAY_HEIGHT);
}

bool chip8display_is_set(display_t *display, uint8_t x, uint8_t y) {
    chip8display_assert_position(x, y);
    return display->pixels[y][x];
}

void chip8display_set(display_t *display, uint8_t x, uint8_t y) {
    chip8display_assert_position(x, y);
    display->pixels[y][x] = true;
}

void chip8display_clear(display_t *display) {
    memset(display->pixels, 0, sizeof(display->pixels));
}

bool chip8display_draw_sprite(display_t *display, uint8_t x, uint8_t y, const uint8_t* sprite, uint8_t height) {
    bool collision = false;

    // iterate sprite Y-axis (top to bottom)
    for (int i = 0; i < height; i++) {
        uint8_t sprite_row = sprite[i];

        // iterate sprite X-axis (left to right)
        for (int j = 0; j < 8; j++) {
            // skip if sprite pixel is off
            if ((sprite_row & (0b10000000 >> j)) == 0)
                continue;

            // check for collision, i.e. sprite pixel on and display pixel on
            if (display->pixels[(y + i) % CHIP8_DISPLAY_HEIGHT][(j + x) % CHIP8_DISPLAY_WIDTH])
                collision = true;
            // sprite pixel is on, XOR with display
            display->pixels[(y + i) % CHIP8_DISPLAY_HEIGHT][(j + x) % CHIP8_DISPLAY_WIDTH] ^= true;            
        }
    }

    return collision;
}
