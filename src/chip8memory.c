#include "chip8memory.h"

void chip8memory_assert_index(uint16_t index) {
    // ensure index within RAM boundaries
    // don't need to check lower bound, since index is unsigned
    assert(index < CHIP8_MEMORY_SIZE);
}

uint8_t chip8memory_get(memory_t *memory, uint16_t index) {
    chip8memory_assert_index(index);
    return memory->buffer[index];
}

void chip8memory_set(memory_t *memory, uint16_t index, uint8_t value) {
    chip8memory_assert_index(index);
    memory->buffer[index] = value;
}
