SLOW DIVISION


@ lab1/func.s
@ Copyright (c) 2018-20 J. M. Spivey

        .syntax unified
        .global func

        .text
        .thumb_func
func:
@ ----------------
@ Two parameters are in registers r0 and r1

        movs r2, #0             @ Set the answer to 0
loop:
        cmp r0, r1              @ If we've reached the end,
        blt done                @ Return
        subs r0, r0, r1         @ Subtract r1 from f0
        adds r2, r2, #1         @ Add 1 to the answer
        b loop                  @ Repeat
done:
@ Result is now in register r0
@ ----------------
        movs r0, r2             @ Register r0 is returned, so move the answer there
        bx lr                   @ Return to the caller

