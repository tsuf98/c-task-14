; file ps.as
MAIN: mov r3 ,LENGTH
LOOP: jmp L1(#-1,r6)
prn             #-5
bne W(r4,r5)
sub r1, r4
bne L3
L1: inc K
bne LOOP(K ,W)
END: stop r2
just a random line
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: .data 22
W: .data 1
W: .data 3 4 5
5A: .string "not a label"
