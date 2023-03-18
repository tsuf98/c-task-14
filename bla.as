;ffsdfs
        ; file ps.as
.entry LENGTH
.extern W
MAIN: mov r3 ,LENGTH
LOOP: jmp L1(#-1,r6)
mcr m1
sub r1, r4
bne END
endmcr
prn #-5
bne LOOP(r4,r3)
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

STR: .string “abcdef”
LENGTH: .data 6,-9,15   
m3

K: .data 22
.extern L3

                 
vvv
