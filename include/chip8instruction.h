#ifndef CHIP8INSTRUCTION_H
#define CHIP8INSTRUCTION_H

#include <assert.h>
#include <stdint.h>

#include "definitions.h"

typedef struct instruction {
    uint16_t opcode;
    uint16_t NNN;
    uint8_t KK;
    uint8_t N;
    uint8_t X;
    uint8_t Y;
} instruction_t;

void chip8instruction_set(instruction_t *instruction, uint16_t opcode);

#endif // CHIP8INSTRUCTION_H
