#ifndef CHIP8DISPLAY_H
#define CHIP8DISPLAY_H

#include <assert.h>
#include <memory.h>
#include <stdbool.h>
#include <stdint.h>

#include "definitions.h"

typedef struct display {
    bool pixels[CHIP8_DISPLAY_HEIGHT][CHIP8_DISPLAY_WIDTH];
} display_t;

bool chip8display_is_set(display_t *display, uint8_t x, uint8_t y);
void chip8display_set(display_t *display, uint8_t x, uint8_t y);
void chip8display_clear(display_t *display);
bool chip8display_draw_sprite(display_t *display, uint8_t x, uint8_t y, const uint8_t* sprite, uint8_t height);

#endif // CHIP8DISPLAY_H
