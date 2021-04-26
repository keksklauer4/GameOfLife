$INCLUDE("../cpp/jp_table.asm")


cseg at 0h
l_start:
	JMP l_init

cseg at 100h
l_init:
	MOV IE, #0
	MOV A, #132
	CALL f_load_dptr
	NOP

l_loop:
	NOP
	NOP
	JMP l_loop


f_key_handler:
	MOV PSW, #00011000b
	MOV A, P0
	CPL A
	JZ l_quit_press_handler ; allow quitting much faster if nothing is pressed
	JB P0.7, l_check_whether_left
	; up pressed
	MOV A, r7
	JZ l_check_whether_left
	DEC r7
	
l_check_whether_left:
	JB P0.6, l_check_whether_down
	; left pressed
	MOV A, r6
	JZ l_check_whether_down
	DEC r6
	
l_check_whether_down:
	JB P0.5, l_check_whether_right
	; down is pressed, INC r7 if allowed
	MOV A, r7
	ANL A, #0x1F ; mask out upper bits
	XRL A, #0x1F ; if A is 1F, then all will be zero
	JZ l_check_whether_right
	INC r7

l_check_whether_right:
	JB P0.4, l_check_whether_toggle_pressed
	; right is pressed, INC r6 if allowed
	MOV A, r6
	ANL A, #0x1F ; mask out upper bits
	XRL A, #0x1F ; if A is 1F, then all will be zero
	JZ l_check_whether_right
	INC r6

l_check_whether_toggle_pressed:
	JB P0.3, l_check_whether_start
	JB r5.0, l_check_whether_start ; if start is set, then disable toggle...
	; toggle pressed, toggle corresponding
	CALL f_toggle_pixel

l_check_whether_start:
	JB P0.2, l_quit_press_handler
	; start pressed
	CPL r5.0

l_quit_press_handler:
	RET

; toggle pixel located at x=r6; y=r7
; consumes A, r0
f_toggle_pixel:
	; set DPTR to correct location
	; for that, set A to correct offset
	MOV A, r7
	ANL A, #00011111b
	RL A
	RL A
	MOV r0, A
	MOV A, r6
	ANL A, #00011000b
	RL A
	SWAP A
	ORL A, r0 ; now A is correct offset
	CALL f_load_dptr
	; DPTR is now set to correct byte
	MOV A, r6 ; get lower three bit to get correct bit mask
	ANL A, #00000111b
	MOV r0, A
	CALL f_create_pixel_mask
	; A is now bit mask to toggle bit
	MOVX A, @DPTR
	XRL A, r0
	MOVX @DPTR, A ; write back toggled value
	RET

; r0 contains bit position (from left)
; consumes A, r0
; r0 must be < 8
f_create_pixel_mask: ; TODO(keksklauer4): consider using SWAP to elimate 4 loops or lookup table
	MOV A, #7
	CRL C
	SUBB A, r0
	MOV r0, A
	MOV A, #10000000b
	CRL C

l_pixel_mask_loop:
	CJNE r0, #0, l_quit_pixel_mask
	RR A ; no need to repeatedly clear flag since only one bit is set
	DEC r0
	SJMP l_pixel_mask_loop

l_quit_pixel_mask:
	RET

; 128 bytes will be copied from 128 offset to matrix contents
f_memcpy_screen_to_matrix:
	PUSH PSW
	MOV PSW, #00010000b ; use register bank 2 for optimized copy

	MOV B, #0x80

l_memcpy_loop:
	; get DPTR on first
	MOV A, B
	CALL f_load_dptr
	; DPTR is on copy from location
	; now copy 8 bytes into r0-r7 while always INC DPTR
	; we bulk copy 8 bytes to heavily (!) optimize the loop
	; performance increase is around (at least) 8 times
	MOVX A, @DPTR
	INC DPTR
	MOV A, r0
	
	MOVX A, @DPTR
	INC DPTR
	MOV A, r1
	
	MOVX A, @DPTR
	INC DPTR
	MOV A, r2
	
	MOVX A, @DPTR
	INC DPTR
	MOV A, r3
	
	MOVX A, @DPTR
	INC DPTR
	MOV A, r4
	
	MOVX A, @DPTR
	INC DPTR
	MOV A, r5
	
	MOVX A, @DPTR
	INC DPTR
	MOV A, r6
	
	MOVX A, @DPTR
	MOV A, r7

	; now set DPTR to dest location
	MOV A, B
	CPL A.7 ; complement msb as it is equivalent to +128 (ignoring overflow)
	CALL f_load_dptr
	; now copy r0-r7 to @DPTR byte after byte
	MOV A, r0
	MOVX @DPTR, A
	INC DPTR

	MOV A, r1
	MOVX @DPTR, A
	INC DPTR

	MOV A, r2
	MOVX @DPTR, A
	INC DPTR

	MOV A, r3
	MOVX @DPTR, A
	INC DPTR

	MOV A, r4
	MOVX @DPTR, A
	INC DPTR

	MOV A, r5
	MOVX @DPTR, A
	INC DPTR

	MOV A, r6
	MOVX @DPTR, A
	INC DPTR

	MOV A, r7
	MOVX @DPTR, A

	MOV A, B
	ADD A, #8 ; add 8 to B because we just read 8 bytes
	MOV B, A
	JNZ l_memcpy_loop ; if A == 0 this means that we completed copying 128 bytes.

	POP PSW
	RET

f_calculate_next_generation:
	; TODO(keksklauer4): Think about how to optimize this

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

