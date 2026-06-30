#ifndef DECODER_H
#define DECODER_H

#include <stdint.h>

/* all the instructions we support, plus UNKNOWN for anything else */
typedef enum {
    ADD, SUB, AND_OP, OR_OP, XOR_OP, SLL, SRL, SRA, SLT, SLTU,
    ADDI, ANDI, ORI, XORI, SLTI, SLTIU, SLLI, SRLI, SRAI,
    LB, LH, LW, LBU, LHU,
    SB, SH, SW,
    BEQ, BNE, BLT, BGE, BLTU, BGEU,
    LUI, AUIPC,
    JAL,
    JALR,
    UNKNOWN
} op_name_t;

/* one decoded instruction */
typedef struct {
    uint32_t raw;
    uint32_t addr;
    op_name_t name;
    int rd;
    int rs1;
    int rs2;
    int imm;
} instruction_t;

void decode(uint32_t raw, uint32_t addr, instruction_t *inst);
void print_instruction(instruction_t *inst, char *out, int out_size);

#endif
