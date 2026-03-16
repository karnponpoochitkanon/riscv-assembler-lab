# 120-line test: heavy arithmetic, memory, branches, loops, and label usage

        # Initialize registers
        addi x1, x0, 0
        addi x2, x0, 1
        addi x3, x0, 2
        addi x4, x0, 3
        addi x5, x0, 4
        addi x6, x0, 5
        addi x7, x0, 6
        addi x8, x0, 7
        addi x9, x0, 8
        addi x10, x0, 9

        # Setup base in x11
        lui  x11, 0x10000
        addi x11, x11, 0

        # Store 16 words to memory
        addi x12, x0, 0
store_loop:
        slt  x13, x12, x10
        beq  x13, x0, store_done
        sw   x12, 0(x11)
        addi x11, x11, 4
        addi x12, x12, 1
        jal  x0, store_loop
store_done:

        # Reverse in-place
        addi x11, x1, 0      # base = 0 for simplicity
        addi x12, x0, 0
        addi x13, x10, 0
rev_loop:
        beq  x12, x13, rev_done
        addi x13, x13, -1
        slli x14, x12, 2
        add  x15, x11, x14
        lw   x16, 0(x15)
        add  x17, x11, x14
        sw   x16, 0(x17)
        addi x12, x12, 1
        jal  x0, rev_loop
rev_done:

        # Fibonacci into memory
        addi x20, x0, 0
        addi x21, x0, 1
        addi x22, x0, 0
        addi x23, x0, 10
fib_loop:
        beq  x22, x23, fib_done
        sw   x21, 0(x11)
        add  x24, x20, x21
        addi x20, x21, 0
        addi x21, x24, 0
        addi x11, x11, 4
        addi x22, x22, 1
        jal  x0, fib_loop
fib_done:

        # Logical instruction coverage
        xori x25, x25, 0xff
        ori  x26, x26, 0x1
        andi x27, x27, 0xf
        slti x28, x28, 100
        sltiu x29, x29, 100
        slli x30, x30, 2
        srli x31, x31, 1
        srai x1, x1, 1

        # Control flow mix
        addi x2, x0, 0
loopA:  addi x2, x2, 1
        bne  x2, x3, loopA

        addi x3, x0, 0
loopB:  addi x3, x3, 1
        blt  x3, x4, loopB

        addi x4, x0, 0
loopC:  addi x4, x4, 1
        bge  x4, x5, loopC

        # Jump and link
        jal  x0, end
middle: addi x5, x5, 1
end:    addi x6, x6, 1
