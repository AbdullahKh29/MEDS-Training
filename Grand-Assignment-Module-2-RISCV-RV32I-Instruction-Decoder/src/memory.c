#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"

/* reads a hex file, one instruction word per line, into prog */
int load_hex_file(const char *filename, program_t *prog) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: could not open file %s\n", filename);
        return -1;
    }

    prog->count = 0;
    char line[MAX_LINE_LEN];

    while (fgets(line, sizeof(line), fp) != NULL) {
        /* skip empty lines */
        if (line[0] == '\n' || line[0] == '\r') {
            continue;
        }

        if (prog->count >= MAX_INSTRUCTIONS) {
            printf("Error: too many instructions in file\n");
            fclose(fp);
            return -1;
        }

        unsigned int word;
        sscanf(line, "%x", &word);
        prog->words[prog->count] = word;
        prog->count++;
    }

    fclose(fp);
    return 0;
}
