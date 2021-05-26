;
; Created by keksklauer4 - 26.04.2021
;


CSEG at 0h
l_start:
	AJMP l_init

cseg at 0Bh
l_timer_zero_handler:
	RETI


cseg at 100h
l_init:
	MOV SP, #0x50
	MOV IE, #0
	CALL f_init_matrices
	CALL f_init_registers
	MOV P1, #0xFF
	MOV P3, #0
	SETB P0.7
	CLR P0.7
	MOV P0, #0
	ORL PSW, #00011000b

l_loop:
	CALL f_iteration
	JMP l_loop

f_iteration:
	;CALL f_clear_screen
	CALL f_draw_screen
	CALL f_massive_sleep
	CALL f_key_handler ; handle key presses
	MOV A, r5
	JNB A.0, l_not_in_simulation_mode
	; start mode
	CALL f_memcpy_screen_to_matrix
	CALL f_calculate_next_generation
l_not_in_simulation_mode:
	; sleep
	RET

f_massive_sleep:
	mov r0, #15

l_sleep_loop:
	; configure timer 0
	; save power by using idle mode
	CLR TCON.4
	MOV TMOD, #0x01
	MOV TL0, #0
	MOV TH0, #0
	MOV IE, #0x82
	SETB TCON.4
	ORL PCON, #0x01
	NOP
	NOP
	DJNZ r0, l_sleep_loop
	RET

; not used
l_outer_outer:
	mov r1, #0
l_outer:
	mov r2, #0
l_inner:
	djnz r2, l_inner

	djnz r1, l_outer
	djnz r0, l_outer_outer
	ret

; consumes A, r0, DPTR
f_init_matrices:
	MOV A, #0
	CALL f_load_dptr
	MOV r0, #0
	MOV A, #0
l_init_matrices_loop:
	MOVX @DPTR, A
	INC DPTR
	DJNZ r0, l_init_matrices_loop
l_end_init_matrices:
	RET

; changes to rb2, sets variables
f_init_registers:
	MOV PSW, #00011000b
	MOV r7, #0 ; y=0
	MOV r6, #0 ; x=0
	MOV r5, #0 ; start=false
	RET

f_key_handler:
	MOV PSW, #00011000b
	MOV A, P0
	;CPL A
	ANL A, #0x7F
	JZ l_quit_press_handler ; allow quitting much faster if nothing is pressed
	MOV B, A
	MOV A, r5
	JB ACC.0, l_check_whether_up ; do not even try to check ACC.6 if on
	MOV A, B
	JNB ACC.6, l_check_whether_up
	ACALL f_multiplexed_selection ; returns here through jump table, then jump there again
	AJMP l_copy_shape_continue

l_check_whether_up:
	JNB P0.0, l_check_whether_left
	; up pressed
	MOV A, r7
	JZ l_check_whether_left
	DEC r7

l_check_whether_left:
	JNB P0.1, l_check_whether_down
	; left pressed
	MOV A, r6
	JZ l_check_whether_down
	DEC r6

l_check_whether_down:
	JNB P0.2, l_check_whether_right
	; down is pressed, INC r7 if allowed
	MOV A, r7
	ANL A, #0x1F ; mask out upper bits
	XRL A, #0x1F ; if A is 1F, then all will be zero
	JZ l_check_whether_right
	INC r7

l_check_whether_right:
	JNB P0.3, l_check_whether_toggle_pressed
	; right is pressed, INC r6 if allowed
	MOV A, r6
	ANL A, #0x1F ; mask out upper bits
	XRL A, #0x1F ; if A is 1F, then all will be zero
	JZ l_check_whether_right
	INC r6

l_check_whether_toggle_pressed:
	JNB P0.4, l_check_whether_start
	MOV A, r5
	JB A.0, l_check_whether_start ; if start is set, then disable toggle...
	CALL f_toggle_pixel
	; toggle pressed, toggle corresponding

l_check_whether_start:
	JNB P0.5, l_quit_press_handler
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
	CALL f_fetch_pixel
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
f_create_pixel_mask:
	MOV A, #10000000b
	CJNE r0, #0, l_prepare_pixel_mask
	SJMP l_quit_pixel_mask
l_prepare_pixel_mask:
	CLR C
l_pixel_mask_loop:
	RR A ; no need to repeatedly clear flag since only one bit is set
	DJNZ r0, l_pixel_mask_loop
l_quit_pixel_mask:
	MOV r0, A
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

; returns to key_handler's caller
f_multiplexed_selection:
	MOV A, r5
	JNB ACC.0, l_loop_await_decision
	RET
l_loop_await_decision:
	NOP
	NOP
	MOV A, P0
	JB ACC.6, l_loop_await_decision

	ANL A, #0x3F
	MOV DPTR, #l_selection_jump_table
	RL A
	RL A
	JMP @A+DPTR
l_copy_shape_continue:
	MOV PSW, #0
	CLR A
	MOVC A, @A+DPTR
	INC DPTR
	MOV r1, A

	; copy loop
l_copy_loop:
	CLR A
	MOVC A, @A+DPTR
	ANL A, #0x1F
	RL A
	RL A
	SETB ACC.7
	MOV B, A
	INC DPTR
	CLR A
	MOVC A, @A+DPTR
	ANL A, #0x18
	RR A
	RR A
	RR A
	ADD A, B
	MOV r0, A
	INC DPTR
	CLR A
	MOVC A, @A+DPTR
	MOVX @R0, A ; write byte

	DJNZ r1, l_copy_loop

	RET



; 128 bytes will be copied from 128 offset to matrix contents
f_memcpy_screen_to_matrix:
	PUSH PSW
	MOV PSW, #00010000b ; use register bank 2 for optimized copy

	MOV R0, #0x80
	MOV R1, #0x00

l_memcpy_loop:
	MOVX A, @R0
	MOVX @R1, A
	CLR A
	MOVX @R0, A
	INC R1
	INC R0
	MOV A, r0
	JNZ l_memcpy_loop

	POP PSW
	RET


f_calculate_next_generation: ; (huge function!)

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
	MOV DPTR, #0 ; set DPTR for lower two rows
	CALL f_copy_from_DPTR_to_regs

; attention, nested loop follows!

l_row_generation_loop: ; (huge loop!)
	MOV DPTR, #l_popcount_lookup ; lookup table will be needed throughout the generation calculation

	; right most is handled separately
	MOV PSW, #00001000b

	; read upper row
	MOV A, r7
	ANL A, #00000011b
	MOVC A, @A+DPTR
	MOV B, A

	; read middle row
	MOV PSW, #00010000b
	MOV A, r3
	ANL A, #00000010b ; we do not want to count the middle (which is the rightmost in this case because we are at the rightmost side of the board)
	MOVC A, @A+DPTR
	ADD A, B
	MOV B, A

	; read lower row
	MOV A, r7
	ANL A, #00000011b
	MOVC A, @A+DPTR
	ADD A, B

	MOV B, A
	ANL A, #11111110b
	JZ l_jump_to_post_draw ; skip drawing if amount neighbors < 2
	MOV A, r3
	JB A.0, l_rightmost_check_survive ; jump if living cell
	; dead cell; check whether it will awake
	MOV A, B
	CJNE A, #3, l_jump_to_post_draw
	JMP l_draw_pixel ; dead cell with three neighbors alive
l_rightmost_check_survive:
	MOV A, B
	ANL A, #11111100b ; if A is not zero now, then amount neighbors > 3 (-> death)
	JNZ l_jump_to_post_draw
	JMP l_draw_pixel ; dead cell with three neighbors alive
l_jump_to_post_draw:
	JMP l_post_draw
l_line_loop: ; (huge loop!)
	; basically the same as in rightmost but with other constants!
	; read upper row
	MOV A, r7
	ANL A, #00000111b
	MOVC A, @A+DPTR
	MOV B, A

	; read middle row
	MOV PSW, #00010000b
	MOV A, r3
	ANL A, #00000101b ; we do not want to count the middle
	MOVC A, @A+DPTR
	ADD A, B
	MOV B, A

	; read lower row
	MOV A, r7
	ANL A, #00000111b
	MOVC A, @A+DPTR
	ADD A, B

	MOV B, A
	ANL A, #11111110b
	JZ l_shift_bits ; skip drawing if amount neighbors < 2
	MOV A, r3
	JB A.1, l_check_survive
	; dead cell; check whether it will awake
	MOV A, B
	CJNE A, #3, l_shift_bits
	SJMP l_draw_pixel ; dead cell with three neighbors will be born (thus draw pixel)
l_check_survive: ; check whether a living cell will survive (has at least 2 living neighbors)
	MOV A, B
	ANL A, #11111100b ; if A is not zero now, then amount neighbors > 3 (-> death)
	JNZ l_shift_bits
	; living cell with 2 <= neighbors <= 3, survives

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
	MOV 0x42, A
	MOV A, #0x07
	CLR C
	SUBB A, 0x42
	MOV DPTR, #l_bitmask_lookup
	MOVC A, @A+DPTR
	MOV r0, A
	MOV A, B
	ADD A, #0x80
	XCH A, r0
	MOV B, A
	MOVX A, @R0
	ORL A, B ; set pixel to high (bitmask is in B)
	MOVX @R0, A
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
	RRC A
	MOV r4, A
	MOV A, r5
	RRC A
	MOV r5, A
	MOV A, r6
	RRC A
	MOV r6, A
	MOV A, r7
	RRC A
	MOV r7, A

	; shift middle
	CLR C
	MOV A, r0
	RRC A
	MOV r0, A
	MOV A, r1
	RRC A
	MOV r1, A
	MOV A, r2
	RRC A
	MOV r2, A
	MOV A, r3
	RRC A
	MOV r3, A

	; shift highest
	MOV PSW, #00001000b
	CLR C
	MOV A, r4
	RRC A
	MOV r4, A
	MOV A, r5
	RRC A
	MOV r5, A
	MOV A, r6
	RRC A
	MOV r6, A
	MOV A, r7
	RRC A
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
	MOV r2, #31
	MOV A, r3
	MOV B, A
	DEC A
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

	; read in lowest
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


; clear screen
; consumes A, r0 and DPTR
f_clear_screen:
	MOV DPTR, #128
	MOV r0, #128
	MOV A, #0

l_clear_screen_loop:
	MOVX @DPTR, A
	INC DPTR
	DJNZ r0, l_clear_screen_loop

	RET


f_draw_screen:
	MOV P1, #0xFF
	MOV P3, #0
	;SETB P0.7
	CLR P0.7
	MOV PSW, #00011000b
	MOV A, r7 ; we have to save the current line of the cursor
	PUSH ACC

	; if r7 > 3 then r0 = 3; else r0 = r7
	MOV r0, A
	ANL A, #0xFC
	JZ l_set_row_mask
	MOV r0, #3

l_set_row_mask:
	MOV A, r7
	MOV r2, #11111110b

	; adjust r7 to make drawing work properly
	; for that we set r7 -= 3
	; if underflow (r7 >= 253) then r7 = 0
	CLR C
	SUBB A, #3
	MOV r7, A
	; if carry clear skip
	JNB C, l_draw_screen_loop
	MOV r7, #0

l_draw_screen_loop:
	CALL f_prepare_line
	CPL A
	MOV B, A
	MOV A, r2
	CPL A
	CLR P0.7
	MOV P1, A
	MOV P3, B
	SETB P0.7
	CLR P0.7
	CPL A
	JNB A.7, l_done_drawing
	RL A
	MOV r2, A
	SJMP l_draw_screen_loop

l_done_drawing:
	POP ACC
	CLR P0.7
	MOV r7, A
	RET

f_prepare_line:
	MOV PSW, #00011000b
	MOV A, r7
	ADD A, r0
	MOV DPTR, #l_row_state_lookup
	; we can now determine the state of the row
	MOVC A, @A+DPTR
	JZ l_normal_line
	DEC A
	JZ l_empty_line
l_edge_line: ; fast indexing in lookup table
	MOV DPTR, #l_edge_line_draw_lookup
	MOV A, r6 ; read x coordinate
	MOVC A, @A+DPTR
	CPL A
	INC r0
	RET

l_empty_line:
	INC r0
	MOV A, #0xFF
	RET

l_normal_line: ; Attention! Tedious line draw logic awaits whoever passes this label!
	; to make it a bit easier, we split this logic again
	; there are three possibilities:
	; 1. x value is far to the left (display edge and potentially empty pixels)
	; 2. x value is more or less centered (display solely from board contents)
	; 3. x value is far to the right (display board contents, edge and potentially empty pixels)
	MOV A, r6 ; read x coordinate
	MOV DPTR, #l_draw_line_state_lookup
	MOVC A, @A+DPTR
	JZ l_centered_normal_line
	DEC A
	JZ l_left_normal_line
l_right_normal_line: ; we see the left edge of our board (3.)
	; r6 >= 28
	MOV A, r6
	CLR C
	SUBB A, #28
	MOV r3, A ; temporarily save that offset for indexing lookup table
	CALL f_fetch_pixel
	MOV A, r3
	MOV r1, A
	INC r1
	MOVX A, @DPTR
	CJNE r1, #0, l_shift_pixels_left
	SJMP l_apply_bitmasks_right_normal_line
l_shift_pixels_left: ; shift them to the left for the border on the right
	CLR C
	RLC A
	DJNZ r1, l_shift_pixels_left

l_apply_bitmasks_right_normal_line:
	; let's use the lookup table to get the bits on the right (the border and empty space)
	MOV r1, A
	MOV DPTR, #l_normal_right_mask_lookup
	MOV A, r3 ; get the offset back
	MOVC A, @A+DPTR
	ORL A, r1 ; merge both parts
	MOV DPTR, #l_reverse_bits_lookup_table
	MOVC A, @A+DPTR
	AJMP l_ret_from_normal_line

l_left_normal_line: ; we see the left edge of our board (1.)
	; => r6 <= 2 -> get mask
	MOV A, r6
	MOV DPTR, #l_normal_left_mask_lookup
	MOVC A, @A+DPTR
	MOV r3, A
	CALL f_fetch_pixel
	MOV A, #3
	CLR C
	SUBB A, r6
	MOV r1, A
	MOVX A, @DPTR

l_shift_right_loop:
	CLR C
	RRC A
	DJNZ r1, l_shift_right_loop

l_shift_right_loop_end:
	ORL A, r3 ; mask in bits we retrieved from lookup table
	MOV DPTR, #l_reverse_bits_lookup_table
	MOVC A, @A+DPTR ; reverse bits in A
	AJMP l_ret_from_normal_line

l_centered_normal_line: ; we are somewhere centered on the board
	MOV A, r6
	PUSH ACC
	CLR C
	SUBB A, #0x03
	MOV r6, A
	CALL f_fetch_pixel
	MOVX A, @DPTR
	MOV B, A
	INC DPTR
	MOVX A, @DPTR
	MOV r1, A
	MOV A, r6 ; get x position to know the amount of times to shift
	ANL A, #00000111b
	MOV r3, A
	MOV A, r1
	CJNE r3, #0, l_centered_line_shift_loop
	SJMP l_prepared_centered_normal_line
	; A contains pixels at location k+1 while B contains pixels at location k
	; we will shift apropriately over both registers
l_centered_line_shift_loop:
	; no need for CLR C because rightmost contents of A (k+1) will be trashed anyways
	RLC A
	XCH A, B
	RLC A ; shift leftmost bit (in Carry) from (k+1) in k
	XCH A, B
	DJNZ r3, l_centered_line_shift_loop

l_prepared_centered_normal_line:
	POP ACC
	MOV r6, A
	XCH A, B ; we need contents of B into A
	MOV DPTR, #l_reverse_bits_lookup_table
	MOVC A, @A+DPTR

l_ret_from_normal_line:
	INC r7 ; normal line means that we have to increment our line counter (we need that because of f_fetch_pixel)
	CPL A
	RET


; consumes A, r1, PSW must be set to 11
f_fetch_pixel:
	MOV A, r7
	ANL A, #00011111b
	RL A
	RL A
	MOV r1, A
	MOV A, r6
	ANL A, #00011000b
	RL A
	SWAP A
	ORL A, r1 ; now A is correct offset
	SETB ACC.7
	; no RET because it just flows right into f_load_dptr

; reg A is offset to some memory location in external ram (pointing to an board byte)
; routine sets DPTR to correct byte location (in at most 3 jumps, 1 ret and 3 MOV(X)s, some arithmetics)
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

INCLUDE "../cpp/jp_table.asm"
INCLUDE "lookup_tables.asm"

END
