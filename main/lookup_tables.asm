;
; Created by keksklauer4 - 26.04.2021
;

ORG 8000h
l_popcount_lookup:
db 0h ; 000
db 1h ; 001
db 1h ; 010
db 2h ; 011
db 1h ; 100
db 2h ; 101
db 2h ; 110
db 3h ; 111


l_bitmask_lookup:
db 00000001b ; 000
db 00000010b ; 001
db 00000100b ; 010
db 00001000b ; 011
db 00010000b ; 100
db 00100000b ; 101
db 01000000b ; 110
db 10000000b ; 111

l_bitmask_lookup_reverse:
db 10000000b ; 111
db 01000000b ; 110
db 00100000b ; 101
db 00010000b ; 100
db 00001000b ; 011
db 00000100b ; 010
db 00000010b ; 001
db 00000001b ; 000


; we use this lookup to quickly determine the type of row to draw
; we index using MOVC A, @A+DPTR with A = current row
; current row is selected position + offset of 3 (because there are dots above) + draw row counter
l_row_state_lookup:
db 1		   ; blank row
db 1		   ; blank row
db 2		   ; edge row
db 0,0,0,0,0,0,0,0 ; rows 0  - 7
db 0,0,0,0,0,0,0,0 ; rows 8  - 15
db 0,0,0,0,0,0,0,0 ; rows 16 - 23
db 0,0,0,0,0,0,0,0 ; rows 24 - 31
db 2		   ; edge row
db 1		   ; blank row
db 1		   ; blank row
db 1		   ; blank row


; lookup table for quick edge line drawing (to reduce tedious logic...)
l_edge_line_draw_lookup:
db 11111100b
db 11111110b
db 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ; fully drawn edge line
db 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ; fully drawn edge line
db 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ; fully drawn edge line
db 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ; fully drawn edge line
db 01111111b
db 00111111b
db 00011111b


; lookup table for quick edge line drawing (to reduce tedious logic...)
l_draw_line_state_lookup:
db 1,1,1 ; leftmost
db 0,0,0,0,0,0,0,0 ; middle
db 0,0,0,0,0,0,0,0 ; middle
db 0,0,0,0,0,0,0,0 ; middle
db 0 ; middle
db 2,2,2,2	   ; rightmost



; lookup table for left normal line
l_normal_left_mask_lookup:
db 00100000b
db 01000000b
db 10000000b



; lookup table for left normal line
l_normal_right_mask_lookup:
db 00000001b
db 00000010b
db 00000100b
db 00001000b


; I was lazy and did not want to change the logic in order to fit the emulator's visualization
; Therefore, I added this lookup table to reverse the bits in A
l_reverse_bits_lookup_table:
db 00000000b, 10000000b, 01000000b, 11000000b, 00100000b, 10100000b, 01100000b, 11100000b
db 00010000b, 10010000b, 01010000b, 11010000b, 00110000b, 10110000b, 01110000b, 11110000b
db 00001000b, 10001000b, 01001000b, 11001000b, 00101000b, 10101000b, 01101000b, 11101000b
db 00011000b, 10011000b, 01011000b, 11011000b, 00111000b, 10111000b, 01111000b, 11111000b
db 00000100b, 10000100b, 01000100b, 11000100b, 00100100b, 10100100b, 01100100b, 11100100b
db 00010100b, 10010100b, 01010100b, 11010100b, 00110100b, 10110100b, 01110100b, 11110100b
db 00001100b, 10001100b, 01001100b, 11001100b, 00101100b, 10101100b, 01101100b, 11101100b
db 00011100b, 10011100b, 01011100b, 11011100b, 00111100b, 10111100b, 01111100b, 11111100b
db 00000010b, 10000010b, 01000010b, 11000010b, 00100010b, 10100010b, 01100010b, 11100010b
db 00010010b, 10010010b, 01010010b, 11010010b, 00110010b, 10110010b, 01110010b, 11110010b
db 00001010b, 10001010b, 01001010b, 11001010b, 00101010b, 10101010b, 01101010b, 11101010b
db 00011010b, 10011010b, 01011010b, 11011010b, 00111010b, 10111010b, 01111010b, 11111010b
db 00000110b, 10000110b, 01000110b, 11000110b, 00100110b, 10100110b, 01100110b, 11100110b
db 00010110b, 10010110b, 01010110b, 11010110b, 00110110b, 10110110b, 01110110b, 11110110b
db 00001110b, 10001110b, 01001110b, 11001110b, 00101110b, 10101110b, 01101110b, 11101110b
db 00011110b, 10011110b, 01011110b, 11011110b, 00111110b, 10111110b, 01111110b, 11111110b
db 00000001b, 10000001b, 01000001b, 11000001b, 00100001b, 10100001b, 01100001b, 11100001b
db 00010001b, 10010001b, 01010001b, 11010001b, 00110001b, 10110001b, 01110001b, 11110001b
db 00001001b, 10001001b, 01001001b, 11001001b, 00101001b, 10101001b, 01101001b, 11101001b
db 00011001b, 10011001b, 01011001b, 11011001b, 00111001b, 10111001b, 01111001b, 11111001b
db 00000101b, 10000101b, 01000101b, 11000101b, 00100101b, 10100101b, 01100101b, 11100101b
db 00010101b, 10010101b, 01010101b, 11010101b, 00110101b, 10110101b, 01110101b, 11110101b
db 00001101b, 10001101b, 01001101b, 11001101b, 00101101b, 10101101b, 01101101b, 11101101b
db 00011101b, 10011101b, 01011101b, 11011101b, 00111101b, 10111101b, 01111101b, 11111101b
db 00000011b, 10000011b, 01000011b, 11000011b, 00100011b, 10100011b, 01100011b, 11100011b
db 00010011b, 10010011b, 01010011b, 11010011b, 00110011b, 10110011b, 01110011b, 11110011b
db 00001011b, 10001011b, 01001011b, 11001011b, 00101011b, 10101011b, 01101011b, 11101011b
db 00011011b, 10011011b, 01011011b, 11011011b, 00111011b, 10111011b, 01111011b, 11111011b
db 00000111b, 10000111b, 01000111b, 11000111b, 00100111b, 10100111b, 01100111b, 11100111b
db 00010111b, 10010111b, 01010111b, 11010111b, 00110111b, 10110111b, 01110111b, 11110111b
db 00001111b, 10001111b, 01001111b, 11001111b, 00101111b, 10101111b, 01101111b, 11101111b
db 00011111b, 10011111b, 01011111b, 11011111b, 00111111b, 10111111b, 01111111b, 11111111b


; patterns for the inserting
; New patterns can easily be added by adding a block below in the form:
	; x.
	; MOV DPTR, #l_my_new_pattern ; adjust this to the correct pattern name
	; RET
; Then add another label that contains the pattern data (see below for explanation)
l_selection_jump_table:
	; 0.
	MOV DPTR, #l_big_oscillator
	RET

	; 1.
	MOV DPTR, #T_nosed_p6
	RET

	; 2.
	MOV DPTR, #l_LWSS
	RET

	; 3.
	MOV DPTR, #l_glider
	RET



; selection for patterns
; Add a new pattern by first writing
; db <amount of bytes to write>
; 		then for each byte of the pattern add
; db <row>, <col>, <byte of your pattern>
;
; Note that row must be 0 <= row <= 31
; and col must be divisible by 8 and 0 <= col <= 24
; 	col can therefore only have values 0, 8, 16 or 24


; pattern for a bug oscillator (src: https://upload.wikimedia.org/wikipedia/commons/9/94/JdlV_osc_3.100.gif)
l_big_oscillator:
	db 10h ; 10h = 16 in decimal. 16 because 16 bytes to draw will follow (each with row, col and the actual pattern byte)
	db 2h, 0h, 00000011b
	db 2h, 8h, 11000000b
	db 3h, 0h, 00000010b
	db 3h, 8h, 01000000b
	db 4h, 0h, 00001110b
	db 4h, 8h, 01110000b
	db 5h, 0h, 00001000b
	db 5h, 8h, 00010000b
	db 6h, 0h, 00001000b
	db 6h, 8h, 00010000b
	db 7h, 0h, 00001110b
	db 7h, 8h, 01110000b
	db 8h, 0h, 00000010b
	db 8h, 8h, 01000000b
	db 9h, 0h, 00000011b
	db 9h, 8h, 11000000b

l_glider:
db 3h ; 3 bytes will be written (the three rows below)
db 3h, 8h, 00000010b
db 4h, 8h, 00000001b
db 5h, 8h, 00000111b

T_nosed_p6:
db 21h
db 3h, 8h, 11000000b
db 3h, 16h, 00011000b
db 3h, 24h, 00000000b

db 4h, 8h, 01000000b
db 4h, 16h, 10101000b
db 4h, 24h, 00000000b

db 5h, 8h, 00000001b
db 5h, 16h, 00010000b
db 5h, 24h, 00000000b

db 6h, 8h, 00000000b
db 6h, 16h, 00000000b
db 6h, 24h, 00000000b

db 7h, 8h, 00101010b
db 7h, 16h, 00001010b
db 7h, 24h, 10000000b

db 8h, 8h, 11101011b
db 8h, 16h, 00011010b
db 8h, 24h, 11100000b

db 9h, 8h, 00101010b
db 9h, 16h, 00001010b
db 9h, 24h, 10000000b

db 10h, 8h, 00000000b
db 10h, 16h, 00000000b
db 10h, 24h, 00000000b

db 11h, 8h, 00000001b
db 11h, 16h, 00010000b
db 11h, 24h, 0000000b

db 12h, 8h, 00000010b
db 12h, 16h, 10101000b
db 12h, 24h, 00000000b

db 13h, 8h, 00000011b
db 13h, 16h, 00011000b
db 13h, 24h, 00000000b

l_LWSS:
db 4h
db 10h, 16h, 00100100b
db 11h, 16h, 01000000b
db 12h, 16h, 01000100b
db 13h, 16h, 01111000b