; file ps.as
MAIN: mov r3 ,LENGTH
LOOP: jmp L9(#-1,r6)
prn #-5, #34
bne W(r4,r5)
sub r1, r4
bne L3
L1: inc K
bne LOOP(K,W)
END: stop
STR: .string "abcdef"