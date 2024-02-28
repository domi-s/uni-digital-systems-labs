        .syntax unified
        .thumb_func
foo:
        subs r0, #1
        bne foo
        bx lr

@args 10
@result 0
