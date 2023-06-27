#ifndef CHIP8MEMORY_H
#define CHIP8MEMORY_H

#include <assert.h>
#include <stdint.h>

#include "definitions.h"

typedef struct memory {
    uint8_t buffer[CHIP8_MEMORY_SIZE];
} memory_t;

void chip8memory_assert_index(uint16_t index);
uint8_t chip8memory_get(memory_t *memory, uint16_t index);
void chip8memory_set(memory_t *memory, uint16_t index, uint8_t value);

#endif // CHIP8MEMORY_H
