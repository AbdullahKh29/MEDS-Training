#include <stdio.h>
#include <string.h>
#include "decoder.h"
#include "common.h"

/* helper to sign extend a value that is 'bits' wide */
int sign_extend(int value, int bits) {
    int shift = 32 - bits;
    return (value << shift) >> shift;
}

/* figures out which instruction this is and fills in the struct */
void decode(uint32_t raw, uint32_t addr, instruction_t *inst) {
    inst->raw = raw;
    inst->addr = addr;
    inst->name = UNKNOWN;
    inst->rd = 0;
    inst->rs1 = 0;
    inst->rs2 = 0;
    inst->imm = 0;

    int opcode = EXTRACT_BITS(raw, 0, 7);
    int rd     = EXTRACT_BITS(raw, 7, 5);
    int funct3 = EXTRACT_BITS(raw, 12, 3);
    int rs1    = EXTRACT_BITS(raw, 15, 5);
    int rs2    = EXTRACT_BITS(raw, 20, 5);
    int funct7 = EXTRACT_BITS(raw, 25, 7);

    inst->rd = rd;
    inst->rs1 = rs1;
    inst->rs2 = rs2;

    if (opcode == OPCODE_R) {
        /* R-type: add, sub, and, or, xor, sll, srl, sra, slt, sltu */
        if (funct3 == 0x0 && funct7 == 0x00) inst->name = ADD;
        else if (funct3 == 0x0 && funct7 == 0x20) inst->name = SUB;
        else if (funct3 == 0x7) inst->name = AND_OP;
        else if (funct3 == 0x6) inst->name = OR_OP;
        else if (funct3 == 0x4) inst->name = XOR_OP;
        else if (funct3 == 0x1) inst->name = SLL;
        else if (funct3 == 0x5 && funct7 == 0x00) inst->name = SRL;
        else if (funct3 == 0x5 && funct7 == 0x20) inst->name = SRA;
        else if (funct3 == 0x2) inst->name = SLT;
        else if (funct3 == 0x3) inst->name = SLTU;
    }
    else if (opcode == OPCODE_I_ARITH) {
        /* I-type arithmetic: addi, andi, ori, xori, slti, sltiu, slli, srli, srai */
        int imm12 = EXTRACT_BITS(raw, 20, 12);
        inst->imm = sign_extend(imm12, 12);

        if (funct3 == 0x0) inst->name = ADDI;
        else if (funct3 == 0x7) inst->name = ANDI;
        else if (funct3 == 0x6) inst->name = ORI;
        else if (funct3 == 0x4) inst->name = XORI;
        else if (funct3 == 0x2) inst->name = SLTI;
        else if (funct3 == 0x3) inst->name = SLTIU;
        else if (funct3 == 0x1) {
            inst->name = SLLI;
            inst->imm = EXTRACT_BITS(raw, 20, 5); /* shamt, not the full imm */
        }
        else if (funct3 == 0x5 && funct7 == 0x00) {
            inst->name = SRLI;
            inst->imm = EXTRACT_BITS(raw, 20, 5);
        }
        else if (funct3 == 0x5 && funct7 == 0x20) {
            inst->name = SRAI;
            inst->imm = EXTRACT_BITS(raw, 20, 5);
        }
    }
    else if (opcode == OPCODE_LOAD) {
        /* I-type load: lb, lh, lw, lbu, lhu */
        int imm12 = EXTRACT_BITS(raw, 20, 12);
        inst->imm = sign_extend(imm12, 12);

        if (funct3 == 0x0) inst->name = LB;
        else if (funct3 == 0x1) inst->name = LH;
        else if (funct3 == 0x2) inst->name = LW;
        else if (funct3 == 0x4) inst->name = LBU;
        else if (funct3 == 0x5) inst->name = LHU;
    }
    else if (opcode == OPCODE_JALR) {
        if (funct3 == 0x0) {
            inst->name = JALR;
            int imm12 = EXTRACT_BITS(raw, 20, 12);
            inst->imm = sign_extend(imm12, 12);
        }
    }
    else if (opcode == OPCODE_STORE) {
        /* S-type: sb, sh, sw. the immediate is split into two pieces */
        int imm_low = EXTRACT_BITS(raw, 7, 5);
        int imm_high = EXTRACT_BITS(raw, 25, 7);
        int imm12 = (imm_high << 5) | imm_low;
        inst->imm = sign_extend(imm12, 12);

        if (funct3 == 0x0) inst->name = SB;
        else if (funct3 == 0x1) inst->name = SH;
        else if (funct3 == 0x2) inst->name = SW;
    }
    else if (opcode == OPCODE_BRANCH) {
        /* B-type: beq, bne, blt, bge, bltu, bgeu
         * the immediate bits are scrambled in a weird order: imm[12|10:5|4:1|11] */
        int bit11 = EXTRACT_BITS(raw, 7, 1);
        int bits4_1 = EXTRACT_BITS(raw, 8, 4);
        int bits10_5 = EXTRACT_BITS(raw, 25, 6);
        int bit12 = EXTRACT_BITS(raw, 31, 1);
        int imm13 = (bit12 << 12) | (bit11 << 11) | (bits10_5 << 5) | (bits4_1 << 1);
        inst->imm = sign_extend(imm13, 13);

        if (funct3 == 0x0) inst->name = BEQ;
        else if (funct3 == 0x1) inst->name = BNE;
        else if (funct3 == 0x4) inst->name = BLT;
        else if (funct3 == 0x5) inst->name = BGE;
        else if (funct3 == 0x6) inst->name = BLTU;
        else if (funct3 == 0x7) inst->name = BGEU;
    }
    else if (opcode == OPCODE_LUI) {
        inst->name = LUI;
        inst->imm = EXTRACT_BITS(raw, 12, 20);
    }
    else if (opcode == OPCODE_AUIPC) {
        inst->name = AUIPC;
        inst->imm = EXTRACT_BITS(raw, 12, 20);
    }
    else if (opcode == OPCODE_JAL) {
        /* J-type: jal. immediate bits are also scrambled: imm[20|10:1|11|19:12] */
        inst->name = JAL;
        int bit20 = EXTRACT_BITS(raw, 31, 1);
        int bits10_1 = EXTRACT_BITS(raw, 21, 10);
        int bit11 = EXTRACT_BITS(raw, 20, 1);
        int bits19_12 = EXTRACT_BITS(raw, 12, 8);
        int imm21 = (bit20 << 20) | (bits19_12 << 12) | (bit11 << 11) | (bits10_1 << 1);
        inst->imm = sign_extend(imm21, 21);
    }
}

/* turns the decoded instruction into a string like "addi x2, x0, 5" */
void print_instruction(instruction_t *inst, char *out, int out_size) {
    switch (inst->name) {
        case ADD:  snprintf(out, out_size, "add x%d, x%d, x%d", inst->rd, inst->rs1, inst->rs2); break;
        case SUB:  snprintf(out, out_size, "sub x%d, x%d, x%d", inst->rd, inst->rs1, inst->rs2); break;
        case AND_OP: snprintf(out, out_size, "and x%d, x%d, x%d", inst->rd, inst->rs1, inst->rs2); break;
        case OR_OP:  snprintf(out, out_size, "or x%d, x%d, x%d", inst->rd, inst->rs1, inst->rs2); break;
        case XOR_OP: snprintf(out, out_size, "xor x%d, x%d, x%d", inst->rd, inst->rs1, inst->rs2); break;
        case SLL:  snprintf(out, out_size, "sll x%d, x%d, x%d", inst->rd, inst->rs1, inst->rs2); break;
        case SRL:  snprintf(out, out_size, "srl x%d, x%d, x%d", inst->rd, inst->rs1, inst->rs2); break;
        case SRA:  snprintf(out, out_size, "sra x%d, x%d, x%d", inst->rd, inst->rs1, inst->rs2); break;
        case SLT:  snprintf(out, out_size, "slt x%d, x%d, x%d", inst->rd, inst->rs1, inst->rs2); break;
        case SLTU: snprintf(out, out_size, "sltu x%d, x%d, x%d", inst->rd, inst->rs1, inst->rs2); break;

        case ADDI:  snprintf(out, out_size, "addi x%d, x%d, %d", inst->rd, inst->rs1, inst->imm); break;
        case ANDI:  snprintf(out, out_size, "andi x%d, x%d, %d", inst->rd, inst->rs1, inst->imm); break;
        case ORI:   snprintf(out, out_size, "ori x%d, x%d, %d", inst->rd, inst->rs1, inst->imm); break;
        case XORI:  snprintf(out, out_size, "xori x%d, x%d, %d", inst->rd, inst->rs1, inst->imm); break;
        case SLTI:  snprintf(out, out_size, "slti x%d, x%d, %d", inst->rd, inst->rs1, inst->imm); break;
        case SLTIU: snprintf(out, out_size, "sltiu x%d, x%d, %d", inst->rd, inst->rs1, inst->imm); break;
        case SLLI:  snprintf(out, out_size, "slli x%d, x%d, %d", inst->rd, inst->rs1, inst->imm); break;
        case SRLI:  snprintf(out, out_size, "srli x%d, x%d, %d", inst->rd, inst->rs1, inst->imm); break;
        case SRAI:  snprintf(out, out_size, "srai x%d, x%d, %d", inst->rd, inst->rs1, inst->imm); break;

        case LB:  snprintf(out, out_size, "lb x%d, %d(x%d)", inst->rd, inst->imm, inst->rs1); break;
        case LH:  snprintf(out, out_size, "lh x%d, %d(x%d)", inst->rd, inst->imm, inst->rs1); break;
        case LW:  snprintf(out, out_size, "lw x%d, %d(x%d)", inst->rd, inst->imm, inst->rs1); break;
        case LBU: snprintf(out, out_size, "lbu x%d, %d(x%d)", inst->rd, inst->imm, inst->rs1); break;
        case LHU: snprintf(out, out_size, "lhu x%d, %d(x%d)", inst->rd, inst->imm, inst->rs1); break;

        case JALR: snprintf(out, out_size, "jalr x%d, x%d, %d", inst->rd, inst->rs1, inst->imm); break;

        case SB: snprintf(out, out_size, "sb x%d, %d(x%d)", inst->rs2, inst->imm, inst->rs1); break;
        case SH: snprintf(out, out_size, "sh x%d, %d(x%d)", inst->rs2, inst->imm, inst->rs1); break;
        case SW: snprintf(out, out_size, "sw x%d, %d(x%d)", inst->rs2, inst->imm, inst->rs1); break;

        case BEQ:  snprintf(out, out_size, "beq x%d, x%d, %d", inst->rs1, inst->rs2, inst->imm); break;
        case BNE:  snprintf(out, out_size, "bne x%d, x%d, %d", inst->rs1, inst->rs2, inst->imm); break;
        case BLT:  snprintf(out, out_size, "blt x%d, x%d, %d", inst->rs1, inst->rs2, inst->imm); break;
        case BGE:  snprintf(out, out_size, "bge x%d, x%d, %d", inst->rs1, inst->rs2, inst->imm); break;
        case BLTU: snprintf(out, out_size, "bltu x%d, x%d, %d", inst->rs1, inst->rs2, inst->imm); break;
        case BGEU: snprintf(out, out_size, "bgeu x%d, x%d, %d", inst->rs1, inst->rs2, inst->imm); break;

        case LUI:   snprintf(out, out_size, "lui x%d, %d", inst->rd, inst->imm); break;
        case AUIPC: snprintf(out, out_size, "auipc x%d, %d", inst->rd, inst->imm); break;

        case JAL: snprintf(out, out_size, "jal x%d, %d", inst->rd, inst->imm); break;

        default:
            snprintf(out, out_size, "UNKNOWN");
            break;
    }
}
