        .syntax unified
        .thumb_func
foo:
        sub sp, #4
        mov r3, lr
        str r3, [sp, #0]
        ldr r0, =#1234
        bl baz
        adds r0, #1
        ldr r3, [sp, #0]
        add sp, #4
        bx r3
        .pool

baz:
        adds r0, r0
        bx lr

@args 123
@result 2469
