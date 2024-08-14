//Mult: a program that computes R2 = R0 * R1

//res=o1
//for(i=o2;i>0;i--){
//	res+=o2
//}

//init
@R0
D=M

@o1
M=D

@R1
D=M

@o2
M=D

//o1>o2 swap(o1,o2);
@o1
D=M-D;
@FI
D;JGE
@o2
D=M
//tmp=o2
@tmp
M=D
//o2=o1
@o1
D=M
@o2
M=D
//o1=tmp
@tmp
D=M
@o1
M=D

(FI)

@res
M=D



(LOOP)
@res
D=M
@o1
D=M+D
@res
M=D
@o2
M=M-1;
D=M
@LOOP
D-1;JGT

@res
D=M
@R2
M=D

(END)
@END
0;JMP