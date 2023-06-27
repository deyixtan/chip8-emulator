#include "chip8.h"

bool chip8_init(chip8_t *chip8, uint8_t *rom_path) {
    //  randomize seed of random generator
    srand(time(NULL));

    // initialize chip8 data structure with zeroes
    memset(chip8, 0, sizeof(chip8_t));

    // load charset buffer at the start of RAM
    const uint8_t charset[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    memcpy(chip8->memory.buffer, charset, sizeof(charset));

    // open ROM file
    FILE *rom = fopen((const char *)rom_path, "rb");
    if (!rom) {
        fprintf(stderr, "Could not open %s ROM file!\n", rom_path);
        return false;
    }

    // check ROM size
    fseek(rom, 0, SEEK_END);
    const long rom_size = ftell(rom);
    const long max_size = sizeof(chip8->memory.buffer) - CHIP8_PROGRAM_ENTRY_POINT;
    rewind(rom);

    if (rom_size > max_size) {
        fprintf(stderr, "ROM file %s too large, ROM size: %ld, Max size: %ld\n", rom_path, rom_size, max_size);
        return false;
    }

    // load ROM to RAM
    if (fread(&chip8->memory.buffer[CHIP8_PROGRAM_ENTRY_POINT], rom_size, 1, rom) != 1) {
        fprintf(stderr, "Could not read ROM file %s into Chip8 memory!\n", rom_path);
        return false;
    }
    fclose(rom);

    // initialise registers
    chip8->registers.PC = CHIP8_PROGRAM_ENTRY_POINT;
    chip8->registers.SP = 0;
    return true;
}

uint16_t chip8_fetch_opcode(chip8_t *chip8) {
    // read 16-bits
    uint8_t byte1 = chip8->memory.buffer[chip8->registers.PC];
    uint8_t byte2 = chip8->memory.buffer[chip8->registers.PC + 1];
    uint16_t opcode = byte1 << 8 | byte2;

    // update PC
    chip8->registers.PC += 2;
    return opcode;
}

void chip8_decode_opcode(chip8_t *chip8, uint16_t opcode) {
    // decode opcode and populate instruction information
    chip8instruction_set(&chip8->instruction, opcode);
}

bool chip8_execute_opcode(chip8_t *chip8) {
    switch ((chip8->instruction.opcode >> 12) & 0xF) {
        case 0x0:
            return chip8_execute_opcode_cat_0(chip8);
        case 0x1:
            return chip8_execute_opcode_cat_1(chip8);
        case 0x2:
            return chip8_execute_opcode_cat_2(chip8);
        case 0x3:
            return chip8_execute_opcode_cat_3(chip8);
        case 0x4:
            return chip8_execute_opcode_cat_4(chip8);
        case 0x5:
            return chip8_execute_opcode_cat_5(chip8);
        case 0x6:
            return chip8_execute_opcode_cat_6(chip8);
        case 0x7:
            return chip8_execute_opcode_cat_7(chip8);
        case 0x8:
            return chip8_execute_opcode_cat_8(chip8);
        case 0x9:
            return chip8_execute_opcode_cat_9(chip8);
        case 0xA:
            return chip8_execute_opcode_cat_A(chip8);
        case 0xB:
            return chip8_execute_opcode_cat_B(chip8);    
        case 0xC:
            return chip8_execute_opcode_cat_C(chip8);  
        case 0xD:
            return chip8_execute_opcode_cat_D(chip8);     
        case 0xE:
            return chip8_execute_opcode_cat_E(chip8);    
        case 0xF:
            return chip8_execute_opcode_cat_F(chip8);                                                                                                       
        default:
            fprintf(stderr, "Warning! Attempted to execute invalid/unimplemented opcode!\n");
            return false;
    }
}

bool chip8_execute_opcode_cat_0(chip8_t *chip8) {
    switch (chip8->instruction.Y) {
        case 0xC:
            fprintf(stderr, "Warning! Attempted to execute unimplemented Super Chip48 opcode 0x%02X!\n", chip8->instruction.KK);
            return true;
        default:
            switch (chip8->instruction.KK) {
                case 0xE0:
                    // 00E0 - CLS
                    //  Clear the display.
                    chip8display_clear(&chip8->display);
                    return true;
                case 0xEE:
                    // 00EE - RET
                    //  Return from a subroutine.
                    //  The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
                    chip8->registers.PC = chip8stack_pop(chip8);
                    return true;
                case 0xFB:
                case 0xFC:
                case 0xFD:
                case 0xFE:
                case 0xFF:
                    fprintf(stderr, "Warning! Attempted to execute unimplemented Super Chip48 opcode 0x%02X!\n", chip8->instruction.KK);
                    return true;
                default:
                    fprintf(stderr, "Warning! Attempted to execute invalid/unimplemented opcode (Category 0)!\n");
                    return false;
            }
    }
}

bool chip8_execute_opcode_cat_1(chip8_t *chip8) {
    // 1nnn - JP addr
    //  Jump to location nnn.
    //  The interpreter sets the program counter to nnn.
    chip8->registers.PC = chip8->instruction.NNN;
    return true;
}

bool chip8_execute_opcode_cat_2(chip8_t *chip8) {
    // 2nnn - CALL addr
    //  Call subroutine at nnn.
    //  The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
    chip8stack_push(chip8, chip8->registers.PC);
    chip8->registers.PC = chip8->instruction.NNN;
    return true;
}

bool chip8_execute_opcode_cat_3(chip8_t *chip8) {
    // 3xkk - SE Vx, byte
    //  Skip next instruction if Vx = kk.
    //  The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
    if (chip8->registers.V[chip8->instruction.X] == chip8->instruction.KK)
        chip8->registers.PC += 2;
    return true;
}

bool chip8_execute_opcode_cat_4(chip8_t *chip8) {
    // 4xkk - SNE Vx, byte
    //  Skip next instruction if Vx != kk.
    //  The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
    if (chip8->registers.V[chip8->instruction.X] != chip8->instruction.KK)
        chip8->registers.PC += 2;
    return true;
}

bool chip8_execute_opcode_cat_5(chip8_t *chip8) {
    // 5xy0 - SE Vx, Vy
    //   Skip next instruction if Vx = Vy.
    //  The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
    if (chip8->registers.V[chip8->instruction.X] == chip8->registers.V[chip8->instruction.Y])
        chip8->registers.PC += 2;
    return true;
}

bool chip8_execute_opcode_cat_6(chip8_t *chip8) {
    // 6xkk - LD Vx, byte
    //   Set Vx = kk.
    //   The interpreter puts the value kk into register Vx.
    chip8->registers.V[chip8->instruction.X] = chip8->instruction.KK;
    return true;
}

bool chip8_execute_opcode_cat_7(chip8_t *chip8) {
    // 7xkk - ADD Vx, byte
    //  Set Vx = Vx + kk.
    //  Adds the value kk to the value of register Vx, then stores the result in Vx.
    chip8->registers.V[chip8->instruction.X] += chip8->instruction.KK;
    return true;
}

bool chip8_execute_opcode_cat_8(chip8_t *chip8) {
    switch (chip8->instruction.N) {
        case 0x0:
            // 8xy0 - LD Vx, Vy
            //  Set Vx = Vy.
            //  Stores the value of register Vy in register Vx.
            chip8->registers.V[chip8->instruction.X] = chip8->registers.V[chip8->instruction.Y];
            return true;
        case 0x1:
            // 8xy1 - OR Vx, Vy
            //  Set Vx = Vx OR Vy.
            //  Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.
            chip8->registers.V[chip8->instruction.X] = chip8->registers.V[chip8->instruction.X] | chip8->registers.V[chip8->instruction.Y];
            return true;
        case 0x2:
            // 8xy2 - AND Vx, Vy
            //  Set Vx = Vx AND Vy.
            //  Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
            chip8->registers.V[chip8->instruction.X] = chip8->registers.V[chip8->instruction.X] & chip8->registers.V[chip8->instruction.Y];
            return true;
        case 0x3:
            // 8xy3 - XOR Vx, Vy
            //  Set Vx = Vx XOR Vy.
            //  Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx.
            chip8->registers.V[chip8->instruction.X] = chip8->registers.V[chip8->instruction.X] ^ chip8->registers.V[chip8->instruction.Y];
            return true;
        case 0x4:
            // 8xy4 - ADD Vx, Vy
            //  Set Vx = Vx + Vy, set VF = carry.
            //  The values of Vx and Vy are added together.
            //  If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0.
            //  Only the lowest 8 bits of the result are kept, and stored in Vx.
            uint16_t result = chip8->registers.V[chip8->instruction.X] + chip8->registers.V[chip8->instruction.Y];
            chip8->registers.V[0xF] = result > 0xFF;
            chip8->registers.V[chip8->instruction.X] = (uint8_t)result;
            return true;
        case 0x5:
            // 8xy5 - SUB Vx, Vy
            //  Set Vx = Vx - Vy, set VF = NOT borrow.
            //  If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
            chip8->registers.V[0xF] = chip8->registers.V[chip8->instruction.X] >= chip8->registers.V[chip8->instruction.Y];
            chip8->registers.V[chip8->instruction.X] = chip8->registers.V[chip8->instruction.X] - chip8->registers.V[chip8->instruction.Y];
            return true;
        case 0x6:
            // 8xy6 - SHR Vx {, Vy}
            // Set Vx = Vx SHR 1.
            // If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
            chip8->registers.V[0xF] = (chip8->registers.V[chip8->instruction.X] >> 0) & 0x1;
            chip8->registers.V[chip8->instruction.X] /= 2;
            return true;
        case 0x7:
            // 8xy7 - SUBN Vx, Vy
            //  Set Vx = Vy - Vx, set VF = NOT borrow.
            //  If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
            chip8->registers.V[0xF] = chip8->registers.V[chip8->instruction.Y] >= chip8->registers.V[chip8->instruction.X];
            chip8->registers.V[chip8->instruction.X] = chip8->registers.V[chip8->instruction.Y] - chip8->registers.V[chip8->instruction.X];
            return true;
        case 0xE:
            // 8xyE - SHL Vx {, Vy}
            //  Set Vx = Vx SHL 1.
            //  If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
            chip8->registers.V[0xF] = (chip8->registers.V[chip8->instruction.X] >> 7) & 0x1;
            chip8->registers.V[chip8->instruction.X] *= 2;
            return true;
        default:
            fprintf(stderr, "Warning! Attempted to execute invalid/unimplemented category 8 opcode 0x%01X!\n", chip8->instruction.N);
            return false;
    }
}

bool chip8_execute_opcode_cat_9(chip8_t *chip8) {
    // 9xy0 - SNE Vx, Vy
    //  Skip next instruction if Vx != Vy.
    //  The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
    if (chip8->registers.V[chip8->instruction.X] != chip8->registers.V[chip8->instruction.Y])
        chip8->registers.PC += 2;
    return true;
}

bool chip8_execute_opcode_cat_A(chip8_t *chip8) {
    // Annn - LD I, addr
    //  Set I = nnn.
    //  The value of register I is set to nnn.
    chip8->registers.I = chip8->instruction.NNN;
    return true;
}

bool chip8_execute_opcode_cat_B(chip8_t *chip8) {
    // Bnnn - JP V0, addr
    //  Jump to location nnn + V0.
    //  The program counter is set to nnn plus the value of V0.
    chip8->registers.PC = chip8->instruction.NNN + chip8->registers.V[0x0];
    return true;
}

bool chip8_execute_opcode_cat_C(chip8_t *chip8) {
    // Cxkk - RND Vx, byte
    //  Set Vx = random byte AND kk.
    //  The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk.
    //  The results are stored in Vx.
    uint8_t result = (rand() % 256) & chip8->instruction.KK;
    chip8->registers.V[chip8->instruction.X] = result;
    return true;
}

bool chip8_execute_opcode_cat_D(chip8_t *chip8) {
    switch (chip8->instruction.N) {
        case 0x0:
            fprintf(stderr, "Warning! Attempted to execute unimplemented Super Chip48 opcode 0x%02X!\n", chip8->instruction.KK);
            return true;
        default:
            // Dxyn - DRW Vx, Vy, nibble
            //  Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
            //  The interpreter reads n bytes from memory, starting at the address stored in I.
            //  These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
            //  Sprites are XORed onto the existing screen.
            //  If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0.
            //  If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen.
            uint8_t x = chip8->registers.V[chip8->instruction.X];
            uint8_t y = chip8->registers.V[chip8->instruction.Y];
            uint8_t *sprite = (uint8_t *)&chip8->memory.buffer[chip8->registers.I];
            chip8->registers.V[0xF] = chip8display_draw_sprite(&chip8->display, x, y, sprite, chip8->instruction.N);
            return true;
    }
}

bool chip8_execute_opcode_cat_E(chip8_t *chip8) {
    switch (chip8->instruction.KK) {
        case 0x9E:
            // Ex9E - SKP Vx
            //  Skip next instruction if key with the value of Vx is pressed.
            //  Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
            if (chip8keyboard_is_key_down(&chip8->keyboard, chip8->registers.V[chip8->instruction.X]))
                chip8->registers.PC += 2;
            return true;
        case 0xA1:
            // ExA1 - SKNP Vx
            //  Skip next instruction if key with the value of Vx is not pressed.
            //  Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
            if (!chip8keyboard_is_key_down(&chip8->keyboard, chip8->registers.V[chip8->instruction.X]))
                chip8->registers.PC += 2;
            return true;
        default:
            fprintf(stderr, "Warning! Attempted to execute invalid/unimplemented category E opcode 0x%02X!\n", chip8->instruction.KK);
            return false;
    }
}

bool chip8_execute_opcode_cat_F(chip8_t *chip8) {
    switch (chip8->instruction.KK) {
        case 0x07:
            // Fx07 - LD Vx, DT
            //  Set Vx = delay timer value.
            //  The value of DT is placed into Vx.
            chip8->registers.V[chip8->instruction.X] = chip8->registers.delay_timer;
            return true;
        case 0x0A:
            // Fx0A - LD Vx, K
            //  Wait for a key press, store the value of the key in Vx.
            //  All execution stops until a key is pressed, then the value of that key is stored in Vx.
            bool valid_key = false;
            int8_t key = -1;
            while (!valid_key) {
                SDL_Event event;
                while(SDL_WaitEvent(&event)) {
                    if (event.type != SDL_KEYDOWN)
                        continue;
                    
                    uint8_t vkey = event.key.keysym.sym;
                    key = chip8keyboard_get_key_index(vkey);
                    if (key != -1) {
                        valid_key = true;
                        break;
                    }
                }
            }
            chip8->registers.V[chip8->instruction.X] = key;
            return true;
        case 0x15:
            // Fx15 - LD DT, Vx
            //  Set delay timer = Vx.
            //  DT is set equal to the value of Vx.    
            chip8->registers.delay_timer = chip8->registers.V[chip8->instruction.X];
            return true;
        case 0x18:
            // Fx18 - LD ST, Vx
            //  Set sound timer = Vx.
            //  ST is set equal to the value of Vx.
            chip8->registers.sound_timer = chip8->registers.V[chip8->instruction.X];
            return true;
        case 0x1E:
            // Fx1E - ADD I, Vx
            // Set I = I + Vx.
            // The values of I and Vx are added, and the results are stored in I.
            chip8->registers.I += chip8->registers.V[chip8->instruction.X];
            return true;
        case 0x29:
            // Fx29 - LD F, Vx
            // Set I = location of sprite for digit Vx.
            // The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx.
            chip8->registers.I = chip8->registers.V[chip8->instruction.X] * CHIP8_CHARSET_SINGLE_SPRITE_HEIGHT; // don't need to ref sprite location since charset loaded at 0x0
            return true;
        case 0x30:
            fprintf(stderr, "Warning! Attempted to execute unimplemented Super Chip48 opcode 0x%02X!\n", chip8->instruction.KK);
            return true;
        case 0x33:
            // Fx33 - LD B, Vx
            //  Store BCD representation of Vx in memory locations I, I+1, and I+2.
            //  The interpreter takes the decimal value of Vx,
            //   and places the hundreds digit in memory at location in I,
            //   the tens digit at location I+1,
            //   and the ones digit at location I+2.
            uint8_t i0 = chip8->registers.V[chip8->instruction.X] / 100 % 10;
            uint8_t i1 = chip8->registers.V[chip8->instruction.X] / 10 % 10;
            uint8_t i2 = chip8->registers.V[chip8->instruction.X] % 10;
            chip8memory_set(&chip8->memory, chip8->registers.I, i0);
            chip8memory_set(&chip8->memory, chip8->registers.I + 1, i1);
            chip8memory_set(&chip8->memory, chip8->registers.I + 2, i2);
            return true;
        case 0x55:
            // Fx55 - LD [I], Vx
            //  Store registers V0 through Vx in memory starting at location I.
            //  The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
            for (int i = 0; i <= chip8->instruction.X; i++)
                chip8memory_set(&chip8->memory, chip8->registers.I + i, chip8->registers.V[i]);
            return true;
        case 0x65:
            // Fx65 - LD Vx, [I]
            //  Read registers V0 through Vx from memory starting at location I.
            //  The interpreter reads values from memory starting at location I into registers V0 through Vx.
            for (int i = 0; i <= chip8->instruction.X; i++)
                chip8->registers.V[i] = chip8memory_get(&chip8->memory, chip8->registers.I + i);
            return true;
        case 0x75:
            fprintf(stderr, "Warning! Attempted to execute unimplemented Super Chip48 opcode 0x%02X!\n", chip8->instruction.KK);
            return true;
        case 0x85:
            fprintf(stderr, "Warning! Attempted to execute unimplemented Super Chip48 opcode 0x%02X!\n", chip8->instruction.KK);
            return true;
        default:
            fprintf(stderr, "Warning! Attempted to execute invalid/unimplemented category F opcode 0x%02X!\n", chip8->instruction.KK);
            return false;
    }
}
