@ lab1/func.s
@ Copyright (c) 2018-20 J. M. Spivey

        .syntax unified
        .global func

        .text
        .thumb_func
func:
@ ----------------
@ Two parameters are in registers r0 and r1
        movs r3, #0
loop:
        movs r2, #1
        ands r2, r2, r1
        beq next
        adds r3, r3, r0
next:
        lsls r0, r0, #1
        lsrs r1, r1, #1
        b loop
done:
@ Result is now in register r0
@ ----------------
        movs r0, r3             @ Register r0 is returned, so move the answer there
        bx lr                   @ Return to the caller

