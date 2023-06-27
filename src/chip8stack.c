#include "chip8stack.h"
#include "chip8.h"

void chip8stack_assert_size(chip8_t *chip8) {
    // don't need to check lower bound, since SP is unsigned
    assert(chip8->registers.SP < sizeof(chip8->stack.buffer));
}

void chip8stack_push(chip8_t *chip8, uint16_t value) {
    chip8->registers.SP += 1;
    chip8stack_assert_size(chip8);  // checks for overflow

    chip8->stack.buffer[chip8->registers.SP] = value;
}

uint16_t chip8stack_pop(chip8_t *chip8) {
    uint16_t ret_addr = chip8->stack.buffer[chip8->registers.SP];
    chip8->registers.SP -= 1;

    chip8stack_assert_size(chip8); // checks for underflow
    return ret_addr;
}
