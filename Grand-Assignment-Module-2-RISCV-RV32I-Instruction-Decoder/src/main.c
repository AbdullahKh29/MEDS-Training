#include <stdio.h>
#include "decoder.h"
#include "memory.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <hex file>\n", argv[0]);
        return 1;
    }

    program_t prog;
    if (load_hex_file(argv[1], &prog) != 0) {
        return 1;
    }

    printf("RISC-V RV32I Instruction Decoder\n");
    printf("================================\n");
    printf("Loaded %d instructions from %s\n\n", prog.count, argv[1]);
    printf("Addr       Hex        Assembly\n");
    printf("---------- ---------- -------------------------\n");

    int valid_count = 0;
    int unknown_count = 0;

    for (int i = 0; i < prog.count; i++) {
        uint32_t addr = i * 4;
        instruction_t inst;
        char asm_str[64];

        decode(prog.words[i], addr, &inst);
        print_instruction(&inst, asm_str, sizeof(asm_str));

        if (inst.name == UNKNOWN) {
            unknown_count++;
        } else {
            valid_count++;
        }

        printf("0x%08X %08X   %s\n", addr, prog.words[i], asm_str);
    }

    printf("\nDecoded %d instructions (%d valid, %d unknown)\n",
           prog.count, valid_count, unknown_count);

    return 0;
}
