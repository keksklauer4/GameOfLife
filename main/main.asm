$INCLUDE("../cpp/jp_table.asm")
$INCLUDE("lookup_tables.asm")

;
; Created by keksklauer4 - 26.04.2021
;

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
	MOV A, r5
	JB A.0, l_check_whether_start ; if start is set, then disable toggle...
	CALL f_toggle_pixel
	; toggle pressed, toggle corresponding

l_check_whether_start:
	JB P0.2, l_quit_press_handler
	; start pressed
	MOV A, r5
	CPL A.0
	MOV r5, A

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
	CLR C
	SUBB A, r0
	MOV r0, A
	MOV A, #10000000b
	CLR C

l_pixel_mask_loop:
	CJNE r0, #0, l_quit_pixel_mask
	RR A ; no need to repeatedly clear flag since only one bit is set
	DEC r0
	SJMP l_pixel_mask_loop

l_quit_pixel_mask:
	RET

f_copy_from_DPTR_to_regs:
	MOVX A, @DPTR
	INC DPTR
	MOV r0, A

	MOVX A, @DPTR
	INC DPTR
	MOV r1, A

	MOVX A, @DPTR
	INC DPTR
	MOV r2, A

	MOVX A, @DPTR
	INC DPTR
	MOV r3, A

	MOVX A, @DPTR
	INC DPTR
	MOV r4, A

	MOVX A, @DPTR
	INC DPTR
	MOV r5, A

	MOVX A, @DPTR
	INC DPTR
	MOV r6, A

	MOVX A, @DPTR
	MOV r7, A
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
	CALL f_copy_from_DPTR_to_regs

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


f_calculate_next_generation: ; (huge function!)
	; steps:
	; init:
	; 	- set lowest row in rb to 0s
	;	  - read middle row in rb
	; 1. read lower two rows into register bank

	; upmost row to 0s
	MOV PSW, #00001000b
	MOV r4, #0
	MOV r5, #0
	MOV r6, #0
	MOV r7, #0

	; use r3 (y) and r2 (x) in rb1 as x and y counter
	MOV r2, #31
	MOV r3, #0

	MOV PSW, #00010000b
	MOV DPTR, #l_jp_table ; set DPTR for lower two rows
	CALL f_copy_from_DPTR_to_regs

; attention, nested loop follows!

l_row_generation_loop: ; (huge loop!)
	MOV DPTR, #l_popcount_lookup ; lookup table will be needed throughout the generation calculation

	; right most is handled separately
	MOV PSW, #00001000b

	; read upper row
	MOV A, r7
	CLR C
	RLC A
	ANL A, #00000011b
	MOVC A, @A+DPTR
	MOV B, A

	; read middle row
	MOV PSW, #00010000b
	MOV A, r3
	CLR C
	RLC A
	ANL A, #00000010b ; we do not want to count the middle (which is the rightmost in this case because we are at the rightmost side of the board)
	MOVC A, @A+DPTR
	ADD A, B
	MOV B, A

	; read lower row
	MOV A, r7
	CLR C
	RLC A
	ANL A, #00000011b
	MOVC A, @A+DPTR
	ADD A, B

	MOV B, A
	ANL A, #11111110b
	JZ l_jump_to_post_draw ; skip drawing if amount neighbors < 2
	MOV A, r3
	JB A.0, l_rightmost_check_survive
	; dead cell; check whether it will awake
	MOV A, B
	CJNE A, #3, l_jump_to_post_draw
	JMP l_draw_pixel ; dead cell with three neighbors alive
l_rightmost_check_survive:
	MOV A, B
	ANL A, #11111100b ; if A is not zero now, then amount neighbors > 3 (-> death)
	JZ l_jump_to_post_draw
	JMP l_draw_pixel ; dead cell with three neighbors alive
l_jump_to_post_draw:
	JMP l_post_draw
l_line_loop: ; (huge loop!)
	; basically the same as in rightmost but with other constants!
	; read upper row
	MOV A, r7
	CLR C
	RLC A
	ANL A, #00000111b
	MOVC A, @A+DPTR
	MOV B, A

	; read middle row
	MOV PSW, #00010000b
	MOV A, r3
	CLR C
	RLC A
	ANL A, #00000101b ; we do not want to count the middle (which is the rightmost in this case because we are at the rightmost side of the board)
	MOVC A, @A+DPTR
	ADD A, B
	MOV B, A

	; read lower row
	MOV A, r7
	CLR C
	RLC A
	ANL A, #00000111b
	MOVC A, @A+DPTR
	ADD A, B

	MOV B, A
	ANL A, #11111110b
	JZ l_shift_bits ; skip drawing if amount neighbors < 2
	MOV A, r3
	JB A.0, l_check_survive
	; dead cell; check whether it will awake
	MOV A, B
	CJNE A, #3, l_shift_bits
	SJMP l_draw_pixel ; dead cell with three neighbors alive
l_check_survive:
	MOV A, B
	ANL A, #11111100b ; if A is not zero now, then amount neighbors > 3 (-> death)
	JZ l_shift_bits
	; dead cell with three neighbors alive

l_draw_pixel:
	; draw a single pixel
	MOV PSW, #00001000b
	MOV A, r3
	ANL A, #00011111b
	RL A
	RL A
	MOV r0, A
	MOV A, r2
	ANL A, #00011000b
	RL A
	SWAP A
	ORL A, r0
	MOV B, A
	MOV A, r2
	ANL A, #00000111b
	MOV DPTR, #l_bitmask_lookup
	MOVC A, @A+DPTR
	MOV r0, A
	MOV A, B
	CALL f_load_dptr
	MOVX A, @DPTR
	ORL A, r0 ; set pixel to high (bitmask is in r0)
	MOVX @DPTR, A
	MOV DPTR, #l_popcount_lookup
	MOV A, r2
	JZ l_done_with_line ; directly quit line drawing if leftmost position
	XRL A, #00011111b
	JZ l_post_draw ; if rightmost skip bit shifting

l_shift_bits:
	MOV PSW, #00010000b
	; shift lowest
	CLR C
	MOV A, r4
	RLC A
	MOV r4, A
	MOV A, r5
	RLC A
	MOV r5, A
	MOV A, r6
	RLC A
	MOV r6, A
	MOV A, r7
	RLC A
	MOV r7, A

	; shift middle
	CLR C
	MOV A, r0
	RLC A
	MOV r0, A
	MOV A, r1
	RLC A
	MOV r1, A
	MOV A, r2
	RLC A
	MOV r2, A
	MOV A, r3
	RLC A
	MOV r3, A

	; shift highest
	MOV PSW, #00001000b
	CLR C
	MOV A, r4
	RLC A
	MOV r4, A
	MOV A, r5
	RLC A
	MOV r5, A
	MOV A, r6
	RLC A
	MOV r6, A
	MOV A, r7
	RLC A
	MOV r7, A


l_post_draw:
	MOV PSW, #00001000b
	MOV A, r2
	JZ l_done_with_line
	DEC r2
	JMP l_line_loop


l_done_with_line:
	; implicity in rb1
	; now increase line
	INC r3
	MOV A, r3
	XRL A, #00100000b
	JNZ l_fill_line_registers
	RET

l_fill_line_registers:
	MOV A, r3
	MOV B, A
	RL A
	RL A
	CALL f_load_dptr

	; read in upper line
	MOVX A, @DPTR
	MOV r4, A
	INC DPTR

	MOVX A, @DPTR
	MOV r5, A
	INC DPTR

	MOVX A, @DPTR
	MOV r6, A
	INC DPTR

	MOVX A, @DPTR
	MOV r7, A
	INC DPTR

	MOV PSW, #00010000b
	; read in middle line
	MOVX A, @DPTR
	MOV r0, A
	INC DPTR

	MOVX A, @DPTR
	MOV r1, A
	INC DPTR

	MOVX A, @DPTR
	MOV r2, A
	INC DPTR

	MOVX A, @DPTR
	MOV r3, A
	INC DPTR

	; check whether lowest line should be copied (line counter is in reg B)
	MOV A, B
	XRL A, #00011111b
	JZ l_set_lowest_zero

	; readin in lowest
	MOVX A, @DPTR
	MOV r4, A
	INC DPTR

	MOVX A, @DPTR
	MOV r5, A
	INC DPTR

	MOVX A, @DPTR
	MOV r6, A
	INC DPTR

	MOVX A, @DPTR
	MOV r7, A

	JMP l_row_generation_loop ; go back to start of loop

l_set_lowest_zero:
	MOV r4, #0
	MOV r5, #0
	MOV r6, #0
	MOV r7, #0
	JMP l_row_generation_loop ; go back to start of loop



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

END