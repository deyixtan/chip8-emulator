#ifndef CHIP8STACK_H
#define CHIP8STACK_H

#include <assert.h>
#include <stdint.h>

#include "definitions.h"

typedef struct chip8 chip8_t; // forward declaration; require SP access

typedef struct stack {
    uint16_t buffer[CHIP8_STACK_DEPTH];
} stack_t;

void chip8stack_assert_size(chip8_t *chip8);
void chip8stack_push(chip8_t *chip8, uint16_t value);
uint16_t chip8stack_pop(chip8_t *chip8);

#endif // CHIP8STACK_H
