cseg at 0h
CLR P0.7
ajmp l_test_button
cseg at 30h
mov A, #88
JMP l_test_button

cseg at 100h
l_init:
MOV TMOD, #0x03
MOV TL0, #0
MOV TL1, #0
MOV TH0, #200
MOV TH1, #0
MOV TCON, #0x10
MOV IE, #0x00
ORL PCON, #0x01

MOV A, #1
SUBB A, #0x85
ANL PSW, #00111011b

MOV A, #156
SUBB A, #123
ANL PSW, #00111011b

MOV A, #15
ADD A, #12
ANL PSW, #00111011b

MOV A, #120
ADD A, #55
ANL PSW, #00111011b

MOV A, #120
ADD A, #150
ANL PSW, #00111011b

SETB C
MOV A, #15
ADDC A, #12
ANL PSW, #00111011b

SETB C
MOV A, #120
ADDC A, #55
ANL PSW, #00111011b

SETB C
MOV A, #120
ADDC A, #150
ANL PSW, #00111011b





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

l_test_button:
MOV A, #3
CLR C
SUBB A, #3
MOV P0, #0
l_button:
MOV A, P0
MOV P1, P0
MOV P3, P0
SETB P0.7
NOP
NOP
CLR P0.7

l_stop_loop:
NOP
SJMP l_button

l_test:
; useless test for the emulator's debugger (testing correct output)
MOV r2, #1
DJNZ r2, l_test
MOV P1, #1
DJNZ P1, l_test
INC r2

MOV DPTR, #0x1234
INC B
INC PSW
DEC B
ADD A, P0
ADDC A, P0
ORL B, A
ORL B, #0x0F
ORL A, B

ANL B, A
ANL B, #0x0F
ANL A, B

MOV B, #0x34
MOV A, #45
XRL B, A
XRL B, #0x0F
XRL A, B

MOV P0, #0x7F
MOV r5, #0x69
MOV P1, P2
MOV R2, #0x18
MOV PSW, R2
MOV P2, @R0

SUBB A, PCON
MOV R4, SBUF
MOV @R1, SCON
PUSH B
POP B
XCH A, P0
MOV B, #100
DJNZ B, l_loop
MOV A, TH0
MOV TL0, A


MOV P3, #0
CLR A
MOV r2, #0
MOV P1, #0x02
MOV P3, #0xf0
SETB P0.7
CLR P0.7
MOV P1,  #1
MOV p3, #1
SETB P0.7
CLR P0.7
MOV P1,  #0x80
MOV p3, #0x80
SETB P0.7
CLR P0.7
MOV P1,  #0x18
MOV p3, #0x18
SETB P0.7
CLR P0.7


l_loop:
l_loopo:
NOP
NOP
CLR P0.7
MOV P1, A
MOV P3, r2
SETB P0.7
INC r2
CALL f_delay
INC A
SJMP l_loopo



f_delay:
MOV r0, #0
l_outer:
MOV r1, #0
l_inner:
NOP
NOP
DJNZ r1, l_inner
DJNZ r0, l_outer

RET

END