// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

(RESET) // 将指针重置到屏幕起始位置
	@SCREEN
	D=A
	@cur_screen_word // 当前操作字的指针
	M=D

(LOOP)	
	@KBD
	D=M
	
	@FILL // 如果有任何按键被按下（RAM[KBD] != 0），则填充字
	D; JGT
	
	@BLANK // 否则空白字
	0; JMP
	
(FILL)
	@cur_screen_word
	A=M
	M=-1
	
	@CHECK
	0; JMP
	
(BLANK)
	@cur_screen_word
	A=M
	M=0
	
	@CHECK
	0; JMP
	
(CHECK) // 检查是否已到达屏幕末端
	@cur_screen_word
	D=M+1
	M=M+1
	@KBD
	D=D-A
	
	@RESET // 如果 RAM[cur_screen_word] == KBD，重置到屏幕起始位置
	D; JEQ
	
	@LOOP // 否则继续填写或者（留空）
	0; JMP

	

