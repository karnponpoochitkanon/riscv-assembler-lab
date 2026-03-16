# 50-line test: more ops, branches, compare, and memory patterns

        lui  x1, 0x10000     # base pointer in upper bits
        addi x1, x1, 0       # x1 holds base

        addi x2, x0, 0       # counter
        addi x3, x0, 10      # limit
        addi x4, x0, 1       # step

loop1:  slt  x5, x2, x3     # x5=1 if x2<x3
        beq  x5, x0, done1
        add  x2, x2, x4
        jal  x0, loop1

done1:  addi x6, x0, 0

        # allocate array at base+0
        addi x7, x1, 0
        addi x8, x0, 0

loop2:  slt  x9, x8, x3      # while i < 10
        beq  x9, x0, done2
        addi x10, x8, 1
        add  x11, x8, x8    # dummy calc
        sw   x8, 0(x7)
        addi x7, x7, 4
        addi x8, x8, 1
        jal  x0, loop2

done2:  # load back sum
        addi x7, x1, 0
        addi x8, x0, 0
        addi x9, x0, 0

loop3:  slt  x10, x8, x3
        beq  x10, x0, done3
        lw   x11, 0(x7)
        add  x9, x9, x11
        addi x7, x7, 4
        addi x8, x8, 1
        jal  x0, loop3

done3:  addi x12, x9, 0  # sum in x12

        # test signed compare
        addi x13, x0, -5
        addi x14, x0, 3
        blt  x13, x14, less
        addi x15, x0, 0
        jal  x0, write_end
less:   addi x15, x0, 1
write_end:
        addi x16, x0, 123
