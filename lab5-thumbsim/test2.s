        .syntax unified
        .thumb_func
foo:
        adds r0, r1
        bx lr

@args 23 34
@result 57
