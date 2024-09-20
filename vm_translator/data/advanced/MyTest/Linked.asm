// File Main.asm
//memory init
//memory segment SP
@256
D=A
@0
M=D
//memory segment LCL
@2048
D=A
@1
M=D
//memory segment ARG
@3000
D=A
@2
M=D
//memory segment THIS
@4000
D=A
@3
M=D
//memory segment THAT
@5000
D=A
@4
M=D
//memory init end
//line number 0
(Main.main)
//set local pointer
@0
D=M
@1
M=D
//set local vars
D=0
//stack push
@0
AM=M+1
A=A-1
M=D
//line number 60
//line number 62
//push constant 1
//stack push
@0
AM=M+1
A=A-1
M=1
//line number 63
// save arg segment to later set the new function arg segment
@1
D=A
@0
D=D-M
D=-D
@temp_new_arg
M=D
@CALL_END_2120071055
D=A
//stack push
@0
AM=M+1
A=A-1
M=D
// save LCL segment
@1
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
// save ARG segment
@2
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
// save THIS segment
@3
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
// save THAT segment
@4
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
//set new ARG
@temp_new_arg
D=M
@2
M=D
@0
D=M
@1
M=D
//jump to function
@Main.new
0;JMP
(CALL_END_2120071055)
//line number 66
//line number 68
//pop local 0
@1
D=M
@0
D=D+A
@pop_normal_segment_temp
M=D
//stack pop
@0
M=M-1
A=M
D=M
@pop_normal_segment_temp
A=M
M=D
//line number 69
//line number 71
//push local 0
@1
D=M
@0
D=D+A
A=D
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
//line number 72
// save arg segment to later set the new function arg segment
@1
D=A
@0
D=D-M
D=-D
@temp_new_arg
M=D
@CALL_END_1675104907
D=A
//stack push
@0
AM=M+1
A=A-1
M=D
// save LCL segment
@1
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
// save ARG segment
@2
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
// save THIS segment
@3
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
// save THAT segment
@4
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
//set new ARG
@temp_new_arg
D=M
@2
M=D
@0
D=M
@1
M=D
//jump to function
@Main.dispose
0;JMP
(CALL_END_1675104907)
//line number 75
//line number 77
//pop temp 0
//stack pop
@0
M=M-1
A=M
D=M
@5
M=D
//line number 78
//line number 80
//push constant 0
//stack push
@0
AM=M+1
A=A-1
M=0
//line number 81
//copy return value to arg 0
@0
A=M-1
D=M
@2
A=M
M=D
//save arg 1 into var
@2
D=M+1
@current_function_arg_1_872383659
M=D
//save return address
@5
D=A
@1
D=D-M
A=-D
D=M
@return_address
M=D
//save local into var
@1
D=M
@saved_local
M=D
//recover memory segment THAT
@saved_local
MD=M-1
A=D
D=M
@4
M=D
//recover memory segment THIS
@saved_local
MD=M-1
A=D
D=M
@3
M=D
//recover memory segment ARG
@saved_local
MD=M-1
A=D
D=M
@2
M=D
//recover memory segment LCL
@saved_local
MD=M-1
A=D
D=M
@1
M=D
//set stack pointer to current function arg 1
@current_function_arg_1_872383659
D=M
@0
M=D
//jump to return address
@return_address
A=M
0;JMP
(Main.new)
//line number 3
//line number 5
//push constant 3
@3
D=A
//stack push
@0
AM=M+1
A=A-1
M=D
//line number 6
// save arg segment to later set the new function arg segment
@1
D=A
@0
D=D-M
D=-D
@temp_new_arg
M=D
@CALL_END_497629365
D=A
//stack push
@0
AM=M+1
A=A-1
M=D
// save LCL segment
@1
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
// save ARG segment
@2
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
// save THIS segment
@3
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
// save THAT segment
@4
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
//set new ARG
@temp_new_arg
D=M
@2
M=D
@0
D=M
@1
M=D
//jump to function
@Memory.alloc
0;JMP
(CALL_END_497629365)
//line number 9
//line number 11
//pop pointer 0
//stack pop
@0
M=M-1
A=M
D=M
@3
M=D
//line number 12
//line number 14
//push constant 1
//stack push
@0
AM=M+1
A=A-1
M=1
//line number 15
//neg
@0
A=M-1
M=-M
//line number 18
//line number 20
//pop this 0
@3
D=M
@0
D=D+A
@pop_normal_segment_temp
M=D
//stack pop
@0
M=M-1
A=M
D=M
@pop_normal_segment_temp
A=M
M=D
//line number 21
//line number 23
//push argument 0
@2
D=M
@0
D=D+A
A=D
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
//line number 24
//line number 26
//pop this 1
@3
D=M
@1
D=D+A
@pop_normal_segment_temp
M=D
//stack pop
@0
M=M-1
A=M
D=M
@pop_normal_segment_temp
A=M
M=D
//line number 27
//line number 29
//push pointer 0
@3
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
//line number 30
//copy return value to arg 0
@0
A=M-1
D=M
@2
A=M
M=D
//save arg 1 into var
@2
D=M+1
@current_function_arg_1_1956487135
M=D
//save return address
@5
D=A
@1
D=D-M
A=-D
D=M
@return_address
M=D
//save local into var
@1
D=M
@saved_local
M=D
//recover memory segment THAT
@saved_local
MD=M-1
A=D
D=M
@4
M=D
//recover memory segment THIS
@saved_local
MD=M-1
A=D
D=M
@3
M=D
//recover memory segment ARG
@saved_local
MD=M-1
A=D
D=M
@2
M=D
//recover memory segment LCL
@saved_local
MD=M-1
A=D
D=M
@1
M=D
//set stack pointer to current function arg 1
@current_function_arg_1_1956487135
D=M
@0
M=D
//jump to return address
@return_address
A=M
0;JMP
//line number 33
(Main.dispose)
//line number 36
//line number 38
//push argument 0
@2
D=M
@0
D=D+A
A=D
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
//line number 39
//line number 41
//pop pointer 0
//stack pop
@0
M=M-1
A=M
D=M
@3
M=D
//line number 42
//line number 44
//push pointer 0
@3
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
//line number 45
// save arg segment to later set the new function arg segment
@1
D=A
@0
D=D-M
D=-D
@temp_new_arg
M=D
@CALL_END_1862153688
D=A
//stack push
@0
AM=M+1
A=A-1
M=D
// save LCL segment
@1
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
// save ARG segment
@2
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
// save THIS segment
@3
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
// save THAT segment
@4
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
//set new ARG
@temp_new_arg
D=M
@2
M=D
@0
D=M
@1
M=D
//jump to function
@Memory.deAlloc
0;JMP
(CALL_END_1862153688)
//line number 48
//line number 50
//pop temp 0
//stack pop
@0
M=M-1
A=M
D=M
@5
M=D
//line number 51
//line number 53
//push constant 0
//stack push
@0
AM=M+1
A=A-1
M=0
//line number 54
//copy return value to arg 0
@0
A=M-1
D=M
@2
A=M
M=D
//save arg 1 into var
@2
D=M+1
@current_function_arg_1_1342017977
M=D
//save return address
@5
D=A
@1
D=D-M
A=-D
D=M
@return_address
M=D
//save local into var
@1
D=M
@saved_local
M=D
//recover memory segment THAT
@saved_local
MD=M-1
A=D
D=M
@4
M=D
//recover memory segment THIS
@saved_local
MD=M-1
A=D
D=M
@3
M=D
//recover memory segment ARG
@saved_local
MD=M-1
A=D
D=M
@2
M=D
//recover memory segment LCL
@saved_local
MD=M-1
A=D
D=M
@1
M=D
//set stack pointer to current function arg 1
@current_function_arg_1_1342017977
D=M
@0
M=D
//jump to return address
@return_address
A=M
0;JMP
//line number 57



// File Test.asm
//line number 0
(Test.new)
//line number 3
//line number 5
//push constant 3
@3
D=A
//stack push
@0
AM=M+1
A=A-1
M=D
//line number 6
// save arg segment to later set the new function arg segment
@1
D=A
@0
D=D-M
D=-D
@temp_new_arg
M=D
@CALL_END_331460312
D=A
//stack push
@0
AM=M+1
A=A-1
M=D
// save LCL segment
@1
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
// save ARG segment
@2
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
// save THIS segment
@3
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
// save THAT segment
@4
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
//set new ARG
@temp_new_arg
D=M
@2
M=D
@0
D=M
@1
M=D
//jump to function
@Memory.alloc
0;JMP
(CALL_END_331460312)
//line number 9
//line number 11
//pop pointer 0
//stack pop
@0
M=M-1
A=M
D=M
@3
M=D
//line number 12
//line number 14
//push constant 1
//stack push
@0
AM=M+1
A=A-1
M=1
//line number 15
//neg
@0
A=M-1
M=-M
//line number 18
//line number 20
//pop this 0
@3
D=M
@0
D=D+A
@pop_normal_segment_temp
M=D
//stack pop
@0
M=M-1
A=M
D=M
@pop_normal_segment_temp
A=M
M=D
//line number 21
//line number 23
//push argument 0
@2
D=M
@0
D=D+A
A=D
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
//line number 24
//line number 26
//pop this 1
@3
D=M
@1
D=D+A
@pop_normal_segment_temp
M=D
//stack pop
@0
M=M-1
A=M
D=M
@pop_normal_segment_temp
A=M
M=D
//line number 27
//line number 29
//push pointer 0
@3
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
//line number 30
//copy return value to arg 0
@0
A=M-1
D=M
@2
A=M
M=D
//save arg 1 into var
@2
D=M+1
@current_function_arg_1_120113809
M=D
//save return address
@5
D=A
@1
D=D-M
A=-D
D=M
@return_address
M=D
//save local into var
@1
D=M
@saved_local
M=D
//recover memory segment THAT
@saved_local
MD=M-1
A=D
D=M
@4
M=D
//recover memory segment THIS
@saved_local
MD=M-1
A=D
D=M
@3
M=D
//recover memory segment ARG
@saved_local
MD=M-1
A=D
D=M
@2
M=D
//recover memory segment LCL
@saved_local
MD=M-1
A=D
D=M
@1
M=D
//set stack pointer to current function arg 1
@current_function_arg_1_120113809
D=M
@0
M=D
//jump to return address
@return_address
A=M
0;JMP
//line number 33
(Test.dispose)
//line number 36
//line number 38
//push argument 0
@2
D=M
@0
D=D+A
A=D
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
//line number 39
//line number 41
//pop pointer 0
//stack pop
@0
M=M-1
A=M
D=M
@3
M=D
//line number 42
//line number 44
//push pointer 0
@3
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
//line number 45
// save arg segment to later set the new function arg segment
@1
D=A
@0
D=D-M
D=-D
@temp_new_arg
M=D
@CALL_END_1353765241
D=A
//stack push
@0
AM=M+1
A=A-1
M=D
// save LCL segment
@1
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
// save ARG segment
@2
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
// save THIS segment
@3
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
// save THAT segment
@4
D=M
//stack push
@0
AM=M+1
A=A-1
M=D
//set new ARG
@temp_new_arg
D=M
@2
M=D
@0
D=M
@1
M=D
//jump to function
@Memory.deAlloc
0;JMP
(CALL_END_1353765241)
//line number 48
//line number 50
//pop temp 0
//stack pop
@0
M=M-1
A=M
D=M
@5
M=D
//line number 51
//line number 53
//push constant 0
//stack push
@0
AM=M+1
A=A-1
M=0
//line number 54
//copy return value to arg 0
@0
A=M-1
D=M
@2
A=M
M=D
//save arg 1 into var
@2
D=M+1
@current_function_arg_1_465565841
M=D
//save return address
@5
D=A
@1
D=D-M
A=-D
D=M
@return_address
M=D
//save local into var
@1
D=M
@saved_local
M=D
//recover memory segment THAT
@saved_local
MD=M-1
A=D
D=M
@4
M=D
//recover memory segment THIS
@saved_local
MD=M-1
A=D
D=M
@3
M=D
//recover memory segment ARG
@saved_local
MD=M-1
A=D
D=M
@2
M=D
//recover memory segment LCL
@saved_local
MD=M-1
A=D
D=M
@1
M=D
//set stack pointer to current function arg 1
@current_function_arg_1_465565841
D=M
@0
M=D
//jump to return address
@return_address
A=M
0;JMP
