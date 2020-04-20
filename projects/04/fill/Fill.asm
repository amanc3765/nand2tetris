// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.
//R0 : Counter till 8K
//R1 : 8K
//R2 : 0/1
//R3 : Screen

    @R0
        M=0
    @8192
        D=A
    @R1
        M=D

(LOOP)
    @SCREEN
        D=A
    @R3
        M=D
    @PAINTSCREEN
        0;JMP
    
(PAINTSCREEN)
    @R1
        D=M             //8K
    @R0
        D=D-M           //8K-R0
    @LOOP
        D;JEQ
    @KBD
        D=M             //Read 0/1
    @WHITE
        D;JEQ
    @R2
        M=-1
    @PAINT
        0;JMP
(WHITE)
    @R2
        M=0
    @PAINT
        0;JMP
(PAINT)
    @R2
        D=M
    @R3
        A=M             //Read screen address
        M=D             //Paint
    @R3
        M=M+1
    @R0
        M=M+1           //inc counter
    @PAINTSCREEN
        0;JMP

