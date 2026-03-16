# 20-line test: basic arithmetic, memory, and control flow

        addi x1, x0, 5      # x1 = 5
        addi x2, x0, 7      # x2 = 7
        add  x3, x1, x2     # x3 = 12
        sub  x4, x2, x1     # x4 = 2
        and  x5, x3, x4     # x5 = 0
        or   x6, x3, x4     # x6 = 14
        xor  x7, x3, x4     # x7 = 14
        sll  x8, x1, x1     # x8 = 5<<5
        srl  x9, x8, x1     # shift right

# memory test
        addi x10, x0, 16    # base address
        sw   x3, 0(x10)     # store x3
        lw   x11, 0(x10)    # load back
        sb   x4, 4(x10)
        lb   x12, 4(x10)

# branch test
        addi x13, x0, 0
loop:   addi x13, x13, 1
        beq  x13, x1, done
        jal  x0, loop

done:   addi x14, x0, 42
