        .syntax unified
        .thumb_func
foo:
        movs r0, #42
        bx lr

@args
@result 42
