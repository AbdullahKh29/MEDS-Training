#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#define MAX_INSTRUCTIONS 1024
#define MAX_LINE_LEN 64

#define NUM_REGISTERS 32

/* gets 'width' bits starting at bit 'shift' from value */
#define EXTRACT_BITS(value, shift, width) (((value) >> (shift)) & ((1 << (width)) - 1))

/* opcodes (bits 6-0 of instruction) */
#define OPCODE_R       0x33
#define OPCODE_I_ARITH 0x13
#define OPCODE_LOAD    0x03
#define OPCODE_STORE   0x23
#define OPCODE_BRANCH  0x63
#define OPCODE_JAL     0x6F
#define OPCODE_JALR    0x67
#define OPCODE_LUI     0x37
#define OPCODE_AUIPC   0x17

#endif
