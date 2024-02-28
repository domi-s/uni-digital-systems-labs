        .syntax unified
        .thumb_func
foo:
        ldr r0, =#1234
        bx lr
        .pool
        .align 2

@args 3
@result 1234
