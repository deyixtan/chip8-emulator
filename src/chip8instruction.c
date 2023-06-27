#include "chip8instruction.h"

void chip8instruction_set(instruction_t *instruction, uint16_t opcode) {
    // update current instruction
    instruction->opcode = opcode;

    // fill out current instruction format
    instruction->NNN = instruction->opcode & 0x0FFF;
    instruction->KK = instruction->opcode & 0x0FF;
    instruction->N = instruction->opcode & 0x0F;
    instruction->X = (instruction->opcode >> 8) & 0x0F;
    instruction->Y = (instruction->opcode >> 4) & 0x0F;
}
