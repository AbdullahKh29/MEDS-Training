# riscv-assembly-challenge

This is my submission for the Module 3 RISC-V ISA assignment. It has 3 assembly
programs (run in Venus) plus 2 written self-study summaries.

## What's in here

- `part1_array_ops.s` — array processing (sum, min, max, count negatives)
- `part2_recursion.s` — recursive merge sort
- `part3_encoding.s` — decodes 6 hand-encoded instructions (opcode/rd/rs1/funct3)
- `screenshots/` — Venus output screenshots for each part/test case
- `docs/ENCODING_WORKSHEET.md` — the hand-encoding work for Part 3
- `docs/PRIVILEGED_SUMMARY.md` — privileged spec summary
- `docs/EXTENSION_SUMMARY.md` — RISC-V extension summary

## How to run

I'm using [Venus](https://venus.cs61c.org/), the browser-based RISC-V simulator.

1. Open Venus
2. Copy-paste the `.s` file you want to run into the editor (or upload it)
3. Hit Assemble, then Run
4. Output shows up in the console at the bottom

No special flags or setup needed, everything's just plain RISC-V assembly.

## Notes on each part

**Part 1 (array ops):** all four functions take `a0 = array pointer` and
`a1 = size`, return the result in `a0`. Main just calls each one and prints
it with a label.

**Part 2 (recursion):** I went with Option A, recursive merge sort. Uses the
standard `merge(array, low, mid, high)` interface. mergeSort is the actual
recursive function, merge itself is just loops (that part isn't recursive,
only the sort is, which I think is what's expected).

**Part 3 (encoding):** picked one instruction per format (R/I/S/B/U/J), hand
encoded them in the worksheet, then wrote the decoder to pull opcode/rd/rs1/funct3
back out using shifts and masks, to check my work against what the program prints.

## Calling convention

Tried to be careful about this since it's called out as the main grading
criterion for Part 2 especially — saving `ra` and any `a`/`s` registers that
need to survive across a `call`, keeping the stack balanced (whatever gets
pushed, gets popped before returning), and not clobbering caller registers
I'm not supposed to touch.

## Screenshots

Screenshots for Part 2 show 3 different test inputs..
