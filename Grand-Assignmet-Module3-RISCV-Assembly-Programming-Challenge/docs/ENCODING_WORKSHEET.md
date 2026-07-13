# RISC-V Encoding Examples

## R-Type

**Instruction:**
```
add x5, x6, x7
```

|Field|Value|
|---- |---- |
|funct7|0000000|
|rs2|00111|
|rs1|00110|
|funct3|000|
|rd|00101|
|opcode|0110011|

**Encoding:** *0x007302B3*

---

## I-Type

**Instruction:**
```
addi x8, x9, 25
```

|Field|Value|
|---- |---- |
|imm[11:0]|000000011001|
|rs1|01001|
|funct3|000|
|rd|01000|
|opcode|0010011|

**Encoding:** *0x01948413*

---

## S-Type

**Instruction:**
```
sw x10, 12(x11)
```

|Field|Value|
|---- |---- |
|imm[11:5]|0000000|
|rs2|01010|
|rs1|01011|
|funct3|010|
|imm[4:0]|01100|
|opcode|0100011|

**Encoding:** *0x00A5A623*

---

## B-Type

**Instruction:**
```
beq x12, x13, 16
```

|Field|Value|
|---- |---- |
|imm[12]|0|
|imm[10:5]|000000|
|rs2|01101|
|rs1|01100|
|funct3|000|
|imm[4:1]|1000|
|imm[11]|0|
|opcode|1100011|

**Encoding:** *0x00D60863*

---

## U-Type

**Instruction:**
```
lui x14, 0x12345
```

|Field|Value|
|---- |---- |
|imm[31:12]|00010010001101000101|
|rd|01110|
|opcode|0110111|

**Encoding:** *0x12345737*

---

## J-Type

**Instruction:**
```
jal x1, 256
```

|Field|Value|
|---- |---- |
|imm[20]|0|
|imm[10:1]|0010000000|
|imm[11]|0|
|imm[19:12]|00000000|
|rd|00001|
|opcode|1101111|

**Encoding:** *0x100000EF*