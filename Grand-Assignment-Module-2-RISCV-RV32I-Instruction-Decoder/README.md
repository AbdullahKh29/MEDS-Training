# RISC-V RV32I Instruction Decoder

This is my grand assignment for MEDS Lab Module 2. It's a command line
program that reads a hex file with RISC-V machine code in it and
prints out the assembly instructions.

## What it does

- Reads a `.hex` file (one 32-bit instruction per line, written in hex)
- Decodes each instruction (figures out what it is and what registers/
  immediates it uses)
- Prints it out like `addi x2, x0, 5`
- If it doesn't recognize an instruction it prints `UNKNOWN` instead of
  crashing

## Project Layout

```
riscv-decoder/
├── README.md
├── Makefile
├── .gitignore
├── include/
│   ├── common.h     # constants and the EXTRACT_BITS macro
│   ├── decoder.h     # the instruction_t struct and decode() prototype
│   └── memory.h      # program_t struct and load_hex_file() prototype
├── src/
│   ├── main.c         # entry point, reads args and prints results
│   ├── decoder.c      # the actual decoding logic
│   └── memory.c       # reads the hex file into memory
├── test/
│   ├── test_decoder.c # unit tests
│   └── test_programs/ # test hex files
│       ├── add.hex
│       ├── r_type.hex
│       ├── i_type.hex
│       ├── branch.hex
│       └── mixed.hex
└── docs/
    └── DESIGN.md
```

## How to build it

You need `gcc` and `make`. Valgrind is optional, only needed for the
`valgrind` target.

```bash
make            # builds bin/riscv-decoder
make debug      # debug build (-O0, no optimizations, easier to debug)
make release    # optimized build
make test       # runs the program on a test file + runs unit tests
make valgrind   # checks for memory leaks
make clean      # deletes build/ and bin/
```

## How to use it

```bash
./bin/riscv-decoder test/test_programs/mixed.hex
```

### Example output

```
$ ./bin/riscv-decoder test/test_programs/mixed.hex
RISC-V RV32I Instruction Decoder
================================
Loaded 8 instructions from test/test_programs/mixed.hex

Addr       Hex        Assembly
---------- ---------- -------------------------
0x00000000 00500113   addi x2, x0, 5
0x00000004 00A00193   addi x3, x0, 10
0x00000008 003100B3   add x1, x2, x3
0x0000000C 40310133   sub x2, x2, x3
0x00000010 0020A023   sw x2, 0(x1)
0x00000014 0000A103   lw x2, 0(x1)
0x00000018 FE209CE3   bne x1, x2, -8
0x0000001C 004000EF   jal x1, 4

Decoded 8 instructions (8 valid, 0 unknown)
```

## Instructions it supports

- R-type: add, sub, and, or, xor, sll, srl, sra, slt, sltu
- I-type (math): addi, andi, ori, xori, slti, sltiu, slli, srli, srai
- I-type (loads): lb, lh, lw, lbu, lhu
- S-type (stores): sb, sh, sw
- B-type (branches): beq, bne, blt, bge, bltu, bgeu
- U-type: lui, auipc
- J-type: jal
- jalr

More details on how the decoding works in `docs/DESIGN.md`.

## Testing

Run `make test` to build everything and run:
1. The program on `test/test_programs/add.hex`
2. `test_decoder.c`, which has 38 small checks comparing decoded output
   against what it should be

There are also a few more hex files in `test/test_programs/` I made to
cover all the different instruction types (`r_type.hex`, `i_type.hex`,
`branch.hex`), plus `mixed.hex` which matches the example from the
assignment sheet.
