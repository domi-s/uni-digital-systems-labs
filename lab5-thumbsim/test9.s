        .syntax unified
        .text
        .thumb_func
foo:
        ldr r1, =x
        ldr r2, [r1]
        str r3, [r1]
        ldr r0, [r1]
        bx lr

        .data
x:
        .word 7
        
@args 0 0 0 222
@result 222
