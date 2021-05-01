cseg at 0h
l_init:
INC A
CPL B.2
CLR A.3
JBC IP.0, l_init
ADD A, B
CPL IP.0


l_fibo:
MOV R0, #0
MOV R1, #1
MOV A, #1
MOVX @R0, A
MOVX @R1, A

l_fibo_iter:
MOVX A,  @R0
MOV R2, A
MOVX A, @R1
ADD A, R2
INC R0
INC R1
MOVX @R1, A
CJNE R0, #10, l_fibo_iter


MOVX A, @R1
XCH A, B
MOVX A, @R0

l_loop:
NOP
NOP
SJMP l_loop




END