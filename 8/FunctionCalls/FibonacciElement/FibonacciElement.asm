// Bootstrap code
@261
D=A
@SP
M=D
@Sys.init
0;JMP
//function Main.fibonacci 0
(Main.fibonacci)
@0
D=A
@SP
M=D+M
//push argument 0
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
//push constant 2
@2
D=A
@SP
A=M
M=D
@SP
M=M+1
//lt
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=M-D
@LT_0
D;JLT
@SP
A=M
M=0
@LT_CONTINUE_0
0;JMP
(LT_0)
@SP
A=M
M=-1
(LT_CONTINUE_0)
@SP
M=M+1
//if-goto Main.fibonacci$N_LT_2
@SP
M=M-1
A=M
D=M
@Main.fibonacci$N_LT_2
D;JNE
//goto Main.fibonacci$N_GE_2
@Main.fibonacci$N_GE_2
0;JMP
//label Main.fibonacci$N_LT_2
(Main.fibonacci$N_LT_2)
//push argument 0
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
//return from Main.fibonacci
@LCL
D=M
@R14
M=D
@5
A=D-A
D=M
@R15
M=D
@SP
M=M-1
A=M
D=M
@ARG
A=M
M=D
@ARG
D=M+1
@SP
M=D
@R14
AM=M-1
D=M
@THAT
M=D
@R14
AM=M-1
D=M
@THIS
M=D
@R14
AM=M-1
D=M
@ARG
M=D
@R14
A=M-1
D=M
@LCL
M=D
@R15
A=M
0;JMP
//label Main.fibonacci$N_GE_2
(Main.fibonacci$N_GE_2)
//push argument 0
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
//push constant 2
@2
D=A
@SP
A=M
M=D
@SP
M=M+1
//sub
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=M-D
@SP
A=M
M=D
@SP
M=M+1
//call Main.fibonacci 1
@Main.fibonacci_Main.fibonacci$ret.0
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@6
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
//goto Main.fibonacci
@Main.fibonacci
0;JMP
//label Main.fibonacci_Main.fibonacci$ret.0
(Main.fibonacci_Main.fibonacci$ret.0)
//push argument 0
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
//push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
//sub
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=M-D
@SP
A=M
M=D
@SP
M=M+1
//call Main.fibonacci 1
@Main.fibonacci_Main.fibonacci$ret.1
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@6
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
//goto Main.fibonacci
@Main.fibonacci
0;JMP
//label Main.fibonacci_Main.fibonacci$ret.1
(Main.fibonacci_Main.fibonacci$ret.1)
//add
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=D+M
@SP
A=M
M=D
@SP
M=M+1
//return from Main.fibonacci
@LCL
D=M
@R14
M=D
@5
A=D-A
D=M
@R15
M=D
@SP
M=M-1
A=M
D=M
@ARG
A=M
M=D
@ARG
D=M+1
@SP
M=D
@R14
AM=M-1
D=M
@THAT
M=D
@R14
AM=M-1
D=M
@THIS
M=D
@R14
AM=M-1
D=M
@ARG
M=D
@R14
A=M-1
D=M
@LCL
M=D
@R15
A=M
0;JMP
//function Sys.init 0
(Sys.init)
@0
D=A
@SP
M=D+M
//push constant 4
@4
D=A
@SP
A=M
M=D
@SP
M=M+1
//call Main.fibonacci 1
@Sys.init_Main.fibonacci$ret.0
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@6
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
//goto Main.fibonacci
@Main.fibonacci
0;JMP
//label Sys.init_Main.fibonacci$ret.0
(Sys.init_Main.fibonacci$ret.0)
//label Sys.init$END
(Sys.init$END)