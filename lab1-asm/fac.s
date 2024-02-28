@ lab1/mul1.s
@ Copyright (c) 2018-20 J. M. Spivey

        .syntax unified
        .global func

        .text
        .thumb_func
mult:
        movs r2, #0             @ Initially z = 0
loop:   
        cmp r1, #0              @ Is y = 0?
        beq done                @ If so, finished

        subs r1, r1, #1         @ Decrease y by 1
        adds r2, r2, r0         @ Increase z by x
        b loop                  @ Repeat
done:
        movs r0, r2             @ Put z in r0
        bx lr                   @ Return to the caller

        .thumb_func
func:                           @ Factorial with mult as a subroutine
        push {r4, r5, lr}
        movs r4, r0             @ Save n in r4
        movs r5, #1             @ Set f to 1
again:
        cmp r4, #0              @ Is n = 0?
        beq finish              @ If so, finished

        movs r0, r5             @ Set f to f * n
        movs r1, r4
        bl mult
        movs r5, r0

        subs r4, r4, #1         @ Decrement n
        b again                 @ And repeat
finish:
        movs r0, r5             @ Return f
        pop {r4, r5, pc}
        

