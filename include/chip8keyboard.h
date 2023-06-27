#ifndef CHIP8KEYBOARD_H
#define CHIP8KEYBOARD_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "definitions.h"

typedef struct keyboard {
    // true = key down, false = key up
    bool keys[CHIP8_KEY_COUNT];
} keyboard_t;

void chip8keyboard_assert_key(uint8_t key);
int8_t chip8keyboard_get_key_index(uint8_t vkey);
bool chip8keyboard_is_key_down(keyboard_t *keyboard, uint8_t key);
void chip8keyboard_key_down(keyboard_t *keyboard, uint8_t key);
void chip8keyboard_key_up(keyboard_t *keyboard, uint8_t key);

#endif // CHIP8KEYBOARD_H
