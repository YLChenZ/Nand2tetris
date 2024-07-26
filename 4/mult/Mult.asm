// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.

//i=1
@i
M=1
//res=0
@res
M=0
(LOOP)
//if (i>R0) goto STOP
@i
D=M
@R0
D=D-M
@STOP
D;JGT
//res=res+R1
@res
D=M
@R1
D=D+M
@res
M=D
//i=i+1
@i
M=M+1
//goto LOOP
@LOOP
0;JMP
(STOP)
//R2=res
@res
D=M
@R2
M=D
//infinite loop
(END)
@END
0;JMP
