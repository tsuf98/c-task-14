;ffsdfs
        ; file ps.as
.entry LENGTH
.extern         W
MM: inc LENGTH
MAN:  mov r7 ,LENGTH
LOOP: jmp L1(#-123,r6)
mcr m1
sub r6, r4
bne END
endmcr
prn #-55
bne LOOP(r5,r7)
m1
mcr m3
sub r1, r4
sub r5, r1
bne END
endmcr
L1: inc K
bne LOOP(K,STR)
END: stop
.entry LOOP             

                STR: .string "abcdef"
LENGTH: .data 6,-9,15   
m3

K: .data 22

.extern L3

                 
