#include <stdio.h>

#include "emulator.h"

bool validate_args(int argc, char** argv) {
    // check for correct arguments usage
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <ROM file>\n", argv[0]);
        return false;
    }
    return true;
}

int main(int argc, char** argv) {
    if (!validate_args(argc, argv))
        goto end;
    
    emulator_t emulator = {0};
    if (!emulator_init(&emulator, (uint8_t *)argv[1]))
        goto end;

    emulator_run(&emulator);

end:
    emulator_cleanup(&emulator);
    return 0;
}
