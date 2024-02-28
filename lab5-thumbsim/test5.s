        .syntax unified
        .thumb_func
foo:
        movs r0, #1
        lsls r0, #31
        bx lr
        .align 4

@args
@result 2147483648
