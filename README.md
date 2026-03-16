# RISC-V Assembler

A single-pass two-pass assembler for the RV32I base integer instruction set, written in C.

## Features

- Assembles all RV32I instruction formats: R, I, S, B, U, J
- Two-pass assembly — forward branch/jump labels work correctly
- Supports `#` line comments and blank lines
- Handles both ALU and load/store memory operand syntax (`rd, imm(rs)`)
- Outputs one hex word per instruction to stdout

## Supported Instructions

| Format | Instructions |
|--------|-------------|
| R | `add`, `sub`, `sll`, `slt`, `sltu`, `xor`, `srl`, `sra`, `or`, `and` |
| I (ALU) | `addi`, `slti`, `sltiu`, `xori`, `ori`, `andi`, `slli`, `srli`, `srai`, `jalr` |
| I (Load) | `lb`, `lh`, `lw`, `lbu`, `lhu` |
| S | `sb`, `sh`, `sw` |
| B | `beq`, `bne`, `blt`, `bge`, `bltu`, `bgeu` |
| U | `lui`, `auipc` |
| J | `jal` |

## Building

```
make
```

This produces the `assembler` binary. Requires `gcc`.

## Usage

```
./assembler <source.s>
```

Each assembled instruction is printed as an 8-digit hex word, one per line:

```
$ ./assembler examples/simple.s
00100093
00200113
00208663
00108093
ff9ff06f
002081b3
```

## Assembly Syntax

```asm
# This is a comment

start:  addi x1, x0, 1       # label + instruction
        addi x2, x0, 2
loop:   beq  x1, x2, end     # branch to label
        addi x1, x1, 1
        jal  x0, loop        # jump to label
end:    add  x3, x1, x2
```

- Registers are written as `x0`–`x31`
- Immediates are decimal or hex (`0x...`)
- Branch/jump targets must be labels defined in the same file
- Load/store use `imm(rs)` syntax: `lw x1, 4(x2)`, `sw x3, -8(x4)`

## Project Structure

```
riscv-assembler-lab/
├── src/
│   ├── main.c          # Entry point: reads file, calls assemble(), prints output
│   ├── assembler.c/h   # Two-pass assembler pipeline
│   ├── encode.c/h      # Bit-level instruction encoding (R/I/S/B/U/J)
│   ├── parser.c/h      # Line parser: label, mnemonic, operands
│   ├── registers.c/h   # Register name → number (x0–x31)
│   ├── instr_table.c/h # Instruction metadata table (opcode, funct3, funct7)
│   └── symbols.c/h     # Label → address symbol table
├── tests/
│   ├── test_registers.c
│   ├── test_parser.c
│   ├── test_instr_table.c
│   ├── test_symbols.c
│   ├── test_encode.c
│   └── test_assembler.c
├── examples/
│   └── simple.s
└── Makefile
```

## Running Tests

Build and run each unit test individually:

```
make test_registers && ./test_registers
make test_parser    && ./test_parser
make test_instr_table && ./test_instr_table
make test_symbols   && ./test_symbols
make test_encode    && ./test_encode
make test_assembler && ./test_assembler
```

Or build and run all at once:

```
make test_registers test_parser test_instr_table test_symbols test_encode test_assembler && \
  ./test_registers && ./test_parser && ./test_instr_table && \
  ./test_symbols   && ./test_encode && ./test_assembler
```

## How It Works

The assembler runs two passes over the input lines:

1. **Pass 1** — scans for labels and records their byte addresses (`index * 4`) in the symbol table.
2. **Pass 2** — encodes each instruction. Branch and jump targets are resolved from the symbol table; the PC-relative offset is computed as `target - pc`.

Each instruction format is encoded by a dedicated function in `encode.c` that places each field at the exact bit position specified by the RV32I spec.

## Cleaning Up

```
make clean
```
