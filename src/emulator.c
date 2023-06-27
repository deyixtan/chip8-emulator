#include "emulator.h"

bool emulator_init(emulator_t *emulator, uint8_t *rom_path) {
    // initializes SDL subsystems
    //  SDL_INIT_VIDEO also initializes SDL_INIT_EVENTS
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        SDL_Log("Coud not initialize SDL subsystems! %s\n", SDL_GetError());
        return false;
    }

    // set up window
    emulator->window = SDL_CreateWindow(
        EMULATOR_WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        CHIP8_DISPLAY_WIDTH * EMULATOR_WINDOW_DISPLAY_SCALE,
        CHIP8_DISPLAY_HEIGHT * EMULATOR_WINDOW_DISPLAY_SCALE,
        SDL_WINDOW_SHOWN
    );
    if (!emulator->window) {
        SDL_Log("Could not create SDL window! %s\n", SDL_GetError());
        return false;
    }

    // set up renderer
    emulator->renderer = SDL_CreateRenderer(emulator->window, -1, SDL_TEXTUREACCESS_TARGET);
    if (!emulator->renderer) {
        SDL_Log("Could not create SDL renderer! %s\n", SDL_GetError());
        return false;
    }

    // initializes chip8 with user specified ROM
    emulator->chip8 = (chip8_t *)malloc(sizeof(chip8_t));
    memset(emulator->chip8, 0, sizeof(chip8_t));
    if (!chip8_init(emulator->chip8, rom_path)) {
        fprintf(stderr, "Could not create Chip8 intepreter with user specified ROM!\n");
        return false;
    }

    return true;
}

void emulator_cleanup(emulator_t *emulator) {
    // check if chip8 was initialized before free-ing
    if (emulator->chip8) {
        free(emulator->chip8);
        emulator->chip8 = NULL;
    }

    // clean up SDL components
    SDL_DestroyRenderer(emulator->renderer);
    SDL_DestroyWindow(emulator->window);
    SDL_Quit(); // shuts down all SDL subsystems
}

void emulator_run(emulator_t *emulator) {
    // emulator event loop
    while (true) {
        uint64_t start_time = SDL_GetPerformanceCounter();
        
        if (!emulator_handle_event(emulator))
            return;
        if (!emulator_execute_next_instruction(emulator))
            return;
        emulator_render_graphics(emulator);
        emulator_handle_timer(emulator);
        Sleep(0.0001); // random sleep to cool down execution

        uint64_t end_time = SDL_GetPerformanceCounter();
        uint64_t elapsed_secs = (end_time - start_time) / SDL_GetPerformanceFrequency();
        uint64_t elapsed_milisecs = elapsed_secs * 1000;

        SDL_Delay(16.7 >= elapsed_milisecs ? 16.7 - elapsed_milisecs : 0);
    }
}

bool emulator_handle_event(emulator_t *emulator) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                fprintf(stderr, "Polled SDL_QUIT event. Quitting now!\n");
                return false;
            case SDL_KEYDOWN: {
                uint8_t vkey = event.key.keysym.sym;
                if (!emulator_handle_vkey(emulator, vkey, true))
                    return false; // user pressed esc
                break;
            }
            case SDL_KEYUP: {
                uint8_t vkey = event.key.keysym.sym;
                if (!emulator_handle_vkey(emulator, vkey, false))
                    return false; // user pressed esc
            }
            default:
                continue;
        };
    }
    return true;
}

bool emulator_handle_vkey(emulator_t *emulator, SDL_KeyCode vkey, bool key_down) {
    // custom keys
    if (vkey == SDLK_ESCAPE)
        return false;

    // chip8 keys
    int8_t key = chip8keyboard_get_key_index(vkey);
    if (key != -1) {
        if (key_down)
            chip8keyboard_key_down(&emulator->chip8->keyboard, key);
        else
            chip8keyboard_key_up(&emulator->chip8->keyboard, key);
    }
    return true;
}

bool emulator_execute_next_instruction(emulator_t *emulator) {
    for (int i = 0; i < EMULATOR_INSTRUCTIONS_PER_EVENT_LOOP; i++) {
        uint16_t opcode = chip8_fetch_opcode(emulator->chip8);
        chip8_decode_opcode(emulator->chip8, opcode);
        bool success = chip8_execute_opcode(emulator->chip8);
        if (!success) {
            fprintf(stderr, "Could not execute the next instruction's opcode with value %d!\n", opcode);
            return false;
        }
    }
    return true;
}

void emulator_render_graphics(emulator_t *emulator) {
    // clears canvas with black
    SDL_SetRenderDrawColor(emulator->renderer, 0, 0, 0, 0);
    SDL_RenderClear(emulator->renderer);

    // make changes to canvas by iterating each pixel
    for (int x = 0; x < CHIP8_DISPLAY_WIDTH; x++) {
        for (int y = 0; y < CHIP8_DISPLAY_HEIGHT; y++) {
            // construct "scaled pixel"
            SDL_Rect r = {
                .x = x * EMULATOR_WINDOW_DISPLAY_SCALE,
                .y = y * EMULATOR_WINDOW_DISPLAY_SCALE,
                .w = EMULATOR_WINDOW_DISPLAY_SCALE,
                .h = EMULATOR_WINDOW_DISPLAY_SCALE
            };

            // draw black pixel if chip8 display at x and y not set
            if (!chip8display_is_set(&emulator->chip8->display, x, y)) {
                SDL_SetRenderDrawColor(emulator->renderer, 0, 0, 0, 0); // white
                SDL_RenderFillRect(emulator->renderer, &r);
                continue;
            }

            // draw black pixel if chip8 display at x and y not set
            SDL_SetRenderDrawColor(emulator->renderer, 255, 255, 255, 0); // black
            SDL_RenderFillRect(emulator->renderer, &r); // draw black pixel
            SDL_SetRenderDrawColor(emulator->renderer, 0, 0, 0, 0); // white
            SDL_RenderDrawRect(emulator->renderer, &r); // draw white outline
        }
    }

    // update canvas with changes made
    SDL_RenderPresent(emulator->renderer);
}

void emulator_handle_timer(emulator_t *emulator) {
    // handle delay timer
    if (emulator->chip8->registers.delay_timer > 0) {
        emulator->chip8->registers.delay_timer -= 1;
    }

    // handle sound timer
    if (emulator->chip8->registers.sound_timer > 0) {
        Beep(EMULATOR_BEEP_FREQUENCY, 10 * emulator->chip8->registers.sound_timer);
        emulator->chip8->registers.sound_timer = 0;
    }
}
