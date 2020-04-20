// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

@R2
    M=0
@R3
    M=0

(LOOP)
    @R3
        D=M
    @R0
        D=M-D
    @END
        D;JEQ
    @R1
        D=M
    @R2
        M=M+D
    @R3
        M=M+1
    @LOOP
        0;JMP

(END)
    @END
        0;JMP


