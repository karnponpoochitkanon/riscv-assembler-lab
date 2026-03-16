# Simple program for the assembler

start:  addi x1, x0, 1
        addi x2, x0, 2
loop:   beq  x1, x2, end
        addi x1, x1, 1
        jal  x0, loop
end:    add  x3, x1, x2
