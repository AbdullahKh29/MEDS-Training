#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include "common.h"

/* holds all the instructions we loaded from the hex file */
typedef struct {
    uint32_t words[MAX_INSTRUCTIONS];
    int count;
} program_t;

int load_hex_file(const char *filename, program_t *prog);

#endif
