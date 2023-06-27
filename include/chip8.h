#ifndef CHIP8_H
#define CHIP8_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>

#include "SDL2/SDL.h"
#include "chip8display.h"
#include "chip8instruction.h"
#include "chip8keyboard.h"
#include "chip8memory.h"
#include "chip8registers.h"
#include "chip8stack.h"

typedef struct chip8 {
    display_t display;
    instruction_t instruction;
    keyboard_t keyboard;
    memory_t memory;
    registers_t registers;
    stack_t stack;
} chip8_t;

bool chip8_init(chip8_t *chip8, uint8_t *rom_path);
uint16_t chip8_fetch_opcode(chip8_t *chip8);
void chip8_decode_opcode(chip8_t *chip8, uint16_t opcode);
bool chip8_execute_opcode(chip8_t *chip8);

// helpers
bool chip8_execute_opcode_cat_0(chip8_t *chip8);
bool chip8_execute_opcode_cat_1(chip8_t *chip8);
bool chip8_execute_opcode_cat_2(chip8_t *chip8);
bool chip8_execute_opcode_cat_3(chip8_t *chip8);
bool chip8_execute_opcode_cat_4(chip8_t *chip8);
bool chip8_execute_opcode_cat_5(chip8_t *chip8);
bool chip8_execute_opcode_cat_6(chip8_t *chip8);
bool chip8_execute_opcode_cat_7(chip8_t *chip8);
bool chip8_execute_opcode_cat_8(chip8_t *chip8);
bool chip8_execute_opcode_cat_9(chip8_t *chip8);
bool chip8_execute_opcode_cat_A(chip8_t *chip8);
bool chip8_execute_opcode_cat_B(chip8_t *chip8);
bool chip8_execute_opcode_cat_C(chip8_t *chip8);
bool chip8_execute_opcode_cat_D(chip8_t *chip8);
bool chip8_execute_opcode_cat_E(chip8_t *chip8);
bool chip8_execute_opcode_cat_F(chip8_t *chip8);

#endif // CHIP8_H
