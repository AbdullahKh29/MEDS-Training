# DESIGN.md

Notes on how I built the decoder and some of the decisions I made.

## How decoding works

Every RISC-V instruction is 32 bits. The first 7 bits (the opcode)
tell you what "type" the instruction is (R-type, I-type, etc), and
based on that I know which other bits to look at.

So in `decode()` I:
1. Pull out the opcode using `EXTRACT_BITS`
2. Pull out rd, rs1, rs2, funct3, funct7 (not all instructions use all
   of these, but it doesn't hurt to grab them anyway)
3. Use a big if/else chain on the opcode, then funct3 (and funct7 for
   a few cases) to figure out exactly which instruction it is
4. Pull out and sign-extend the immediate if the instruction needs one

## The EXTRACT_BITS macro

```c
#define EXTRACT_BITS(value, shift, width) (((value) >> (shift)) & ((1 << (width)) - 1))
```

This shifts the value right so the bits I want are at the bottom, then
masks off everything except the number of bits I asked for. E.g.
`EXTRACT_BITS(raw, 12, 3)` gets funct3 (3 bits starting at bit 12).

## Sign extension

Immediates in RISC-V are stored as fewer than 32 bits, but they can be
negative (like a branch jumping backwards). I wrote a small helper:

```c
int sign_extend(int value, int bits) {
    int shift = 32 - bits;
    return (value << shift) >> shift;
}
```

This shifts the value all the way left so the sign bit ends up in bit
31, then shifts it back right. Because it's a signed int, the right
shift fills in with 1s if the number is negative, which is exactly
what sign extension means.

## ADD vs SUB, and SRL vs SRA

These pairs use the exact same opcode and funct3 — the only
difference is bit 30 of the instruction (which is part of funct7).
If funct7 is 0x20 it's the "subtract-like" version, otherwise it's
the normal one. I check this with `funct7 == 0x20` in the if/else
chain.

## Branch and JAL immediates are scrambled

This was the hardest part. For branches (B-type) and jal (J-type),
the immediate bits aren't stored in order — they're split up and
stored in a weird order in the instruction. I think this is done so
the hardware can decode some of the fields the same way no matter what
instruction type it is, but it makes the software side more annoying.

For branches the bit order is `imm[12|10:5|4:1|11]`, so I pull out
each piece separately, shift them to where they belong, OR them
together, and then sign extend the whole thing. Same idea for jal but
with a different bit order: `imm[20|10:1|11|19:12]`.

I added comments above both of these in `decoder.c` since they're easy
to mess up.

## Unknown instructions

I default `inst->name` to `UNKNOWN` at the very start of `decode()`,
before any of the if/else checks run. That way if none of the if/else
branches match (opcode doesn't match anything, or funct3 doesn't
match within a branch), it just stays as UNKNOWN and
`print_instruction()` prints `"UNKNOWN"` for it. So I don't need a
separate error case — it's just whatever the if/else chain didn't
catch.

## Note about the DEADBEEF example in the assignment

The assignment sheet shows `0xDEADBEEF` decoding to `UNKNOWN`, but I
checked and the last 7 bits of `0xDEADBEEF` actually match the opcode
for `jal`. So technically my decoder would print something like
`jal x29, -150038` for that exact value instead of `UNKNOWN`, since
it really is a valid (if weird) jal encoding. I used
`0xFFFFFFFF` and `0x00000000` in my own unknown test instead since
those opcodes really aren't used by anything.

## Memory leaks

I don't use any `malloc` anywhere — the `program_t` struct just has a
fixed-size array in it, so everything is on the stack. That's probably
why valgrind shows zero leaks, there's nothing being allocated on the
heap to begin with.
