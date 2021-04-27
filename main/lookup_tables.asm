ORG 3000h
l_popcount_lookup:
db 0h ; 000
db 1h ; 001
db 1h ; 010
db 2h ; 011
db 1h ; 100
db 2h ; 101
db 2h ; 110
db 3h ; 111


ORG 3050h
l_bitmask_lookup:
db 00000001b ; 000
db 00000010b ; 001
db 00000100b ; 010
db 00001000b ; 011
db 00010000b ; 100
db 00100000b ; 101
db 01000000b ; 110
db 10000000b ; 111

ORG 3058h
l_bitmask_lookup_reverse:
db 10000000b ; 111
db 01000000b ; 110
db 00100000b ; 101
db 00010000b ; 100
db 00001000b ; 011
db 00000100b ; 010
db 00000010b ; 001
db 00000001b ; 000

END