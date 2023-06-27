#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Definitions for Emulator Window
#define EMULATOR_WINDOW_TITLE "Chip8 Emulator"
#define EMULATOR_WINDOW_DISPLAY_SCALE 20
#define EMULATOR_INSTRUCTIONS_PER_EVENT_LOOP 30
#define EMULATOR_BEEP_FREQUENCY 15000

// Definitions from Chip8 Specifications
#define CHIP8_DISPLAY_WIDTH 64
#define CHIP8_DISPLAY_HEIGHT 32
#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_PROGRAM_ENTRY_POINT 512 // or 0x200
#define CHIP8_CHARSET_SINGLE_SPRITE_HEIGHT 5

#define CHIP8_STACK_DEPTH 16
#define CHIP8_REGISTER_COUNT 16
#define CHIP8_KEY_COUNT 16

#endif // DEFINITIONS_H