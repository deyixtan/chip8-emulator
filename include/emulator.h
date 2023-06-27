#ifndef EMULATOR_H
#define EMULATOR_H

#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <windows.h>

#include "SDL2/SDL.h"
#include "chip8.h"
#include "definitions.h"

typedef struct emulator {
    SDL_Window *window;
    SDL_Renderer *renderer;
    chip8_t *chip8;
} emulator_t;

bool emulator_init(emulator_t *emulator, uint8_t *rom_path);
void emulator_cleanup(emulator_t *emulator);
void emulator_run(emulator_t *emulator);

// helpers
bool emulator_handle_event(emulator_t *emulator);
bool emulator_handle_vkey(emulator_t *emulator, SDL_KeyCode vkey, bool key_down);
bool emulator_execute_next_instruction(emulator_t *emulator);
void emulator_render_graphics(emulator_t *emulator);
void emulator_handle_timer(emulator_t *emulator);

#endif // EMULATOR_H
