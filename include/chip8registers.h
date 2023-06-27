#ifndef CHIP8REGISTERS_H
#define CHIP8REGISTERS_H

#include <stdint.h>

#include "definitions.h"

typedef struct registers {
    uint8_t V[CHIP8_REGISTER_COUNT];
    uint16_t I;
    uint8_t delay_timer;
    uint8_t sound_timer;

    // pseudo-registers (not accessible to Chip8 programs)
    uint16_t PC; // program counter
    uint8_t SP; // stack pointer
} registers_t;

#endif // CHIP8REGISTERS_H
