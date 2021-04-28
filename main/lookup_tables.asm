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
db 00111111b
db 01111111b
db 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ; fully drawn edge line
db 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ; fully drawn edge line
db 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ; fully drawn edge line
db 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ; fully drawn edge line
db 11111110b
db 11111100b
db 11111000b


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
