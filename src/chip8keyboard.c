#include "chip8keyboard.h"

const uint8_t keyboard_key_vkey_map[CHIP8_KEY_COUNT] = {
    0x78, // SDLK_x
    0x31, // SDLK_1
    0x32, // SDLK_2
    0x33, // SDLK_3
    0x71, // SDLK_q
    0x77, // SDLK_w
    0x65, // SDLK_e
    0x61, // SDLK_a
    0x73, // SDLK_s
    0x64, // SDLK_d
    0x7A, // SDLK_z
    0x63, // SDLK_c
    0x34, // SDLK_4
    0x72, // SDLK_r
    0x66, // SDLK_f
    0x76, // SDLK_v
};

void chip8keyboard_assert_key(uint8_t key)  {
    // don't need to check lower bound, since key is unsigned
    assert(key < CHIP8_KEY_COUNT);
}

int8_t chip8keyboard_get_key_index(uint8_t vkey) {
    // Iterates through the entire key map
    // Find key that is mapped to vkey
    for (uint8_t i = 0; i < CHIP8_KEY_COUNT; i++) {
        if (keyboard_key_vkey_map[i] == vkey) {
            // key found in map
            return i;
        }
    }

    // key not found in map
    return -1;
}

bool chip8keyboard_is_key_down(keyboard_t *keyboard, uint8_t key) {
    chip8keyboard_assert_key(key);
    return keyboard->keys[key];
}

void chip8keyboard_key_down(keyboard_t *keyboard, uint8_t key) {
    chip8keyboard_assert_key(key);
    keyboard->keys[key] = true;
}

void chip8keyboard_key_up(keyboard_t *keyboard, uint8_t key) {
    chip8keyboard_assert_key(key);
    keyboard->keys[key] = false;
}
