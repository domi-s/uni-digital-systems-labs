MULTIPLYING R1 WITH 10


@ lab1/func.s
@ Copyright (c) 2018-20 J. M. Spivey

        .syntax unified
        .global func

        .text
        .thumb_func
func:
@ ----------------
@ Two parameters are in registers r0 and r1

        lsls r0, r1, #3         @ r0 := 8 * r1
        lsls r1, r1, #1         @ r1 := 2 * r1
        adds r0, r0, r1         @ Add 2 * r1 and 8 * r1 = 10 * r1, result in r0

@ Result is now in register r0
@ ----------------
        bx lr                   @ Return to the caller

