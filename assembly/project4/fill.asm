// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

@8192
D=A
@SCREEN_SIZE
M=D
@SCREEN_IS_BLACK
M=0

(INFINITE_LOOP)

@KBD
D=M
@KEY_IS_PRESSED
D;JNE
@KEY_IS_NOT_PRESSED
0;JMP
(KEY_IS_PRESSED)
	@SCREEN_IS_BLACK
	D=M
	@MAKE_SCREEN_BLACK
	D;JEQ
	@INFINITE_LOOP
	0;JMP
(KEY_IS_NOT_PRESSED)
	@SCREEN_IS_BLACK
	D=M
	@MAKE_SCREEN_WHITE
	D-1;JEQ
@INFINITE_LOOP
0;JMP

(MAKE_SCREEN_BLACK)
	@i
	M=0
	D=M

	(LOOP)
	@SCREEN
	A=D+A
	M=-1
	@i
	DM=M+1
	@SCREEN_SIZE
	D=M-D
	@LOOP
	D;JGE
@SCREEN_IS_BLACK
M=1
@INFINITE_LOOP
0;JMP



(MAKE_SCREEN_WHITE)
	@i
	M=0
	D=M

	(LOOP2)
	@SCREEN
	A=D+A
	M=0
	@i
	DM=M+1
	@SCREEN_SIZE
	D=M-D
	@LOOP2
	D;JGE
@SCREEN_IS_BLACK
M=0

@INFINITE_LOOP
0;JMP