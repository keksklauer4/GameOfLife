$INCLUDE("../cpp/jp_table.asm")


cseg at 0h
l_start:
	JMP l_init

cseg at 100h
l_init:
	MOV IE, #0h
	MOV A, #132
	CALL f_load_dptr
	NOP

l_loop:
	NOP
	NOP
	JMP l_loop


; reg A is offset to some memory location in external ram (pointing to an board byte)
; routine sets DPTR to correct byte location (in at most 3 jumps, 1 ret and 3 MOV(X)s, some arithmetics))
; -> much faster than INC DPTR in a loop!
f_load_dptr:
	JB A.7, l_1x_bit
	JB A.6, l_01_bit
l_00_bit:
	MOV DPTR, #l_jp_table + 0 * 256
	RL A
	RL A
	JMP @A+DPTR
l_01_bit:
	MOV DPTR, #l_jp_table + 1 * 256
	RL A
	CLR C
	RLC A ; one byte shorter than ANL A, #3Fh -> RL -> RL
	JMP @A+DPTR

l_1x_bit:
	JB A.6, l_11_bit
l_10_bit:
	MOV DPTR, #l_jp_table + 2 * 256
	CLR C
	RLC A
	RL A
	JMP @A+DPTR
l_11_bit:
	MOV DPTR, #l_jp_table + 3 * 256
	ANL A, #3Fh
	RL A
	RL A
	JMP @A+DPTR
	; no RET needed!

