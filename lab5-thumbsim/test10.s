        .syntax unified
        .text
        .thumb_func
foo:
        movs r0, #1
        lsls r0, 31
        mvns r1, r0
        cmp r0, r1
        bge skip
        adds r0, #1
skip:   
        bx lr

@args 12 34
@result 2147483649
