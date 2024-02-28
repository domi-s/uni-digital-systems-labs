@ Long bl instructions

        .syntax unified
        .text

        .thumb_func
foo:
        mov r7, lr              @ Save return address
        bl baz
        bx r7

        .thumb_func
ggg:
        movs r0, #3
        bx lr

        .thumb_func
fff:
        sub sp, #4
        str r7, [sp]
        mov r7, lr
        bl ggg
        adds r0, r0, #20
        movs r3, r7
        ldr r7, [sp]
        add sp, #4
        bx r3

        .org 0x2346
        .thumb_func
baz:
        sub sp, #4
        str r7, [sp]
        mov r7, lr
        bl fff
        adds r0, r0, #100
        mov r3, r7
        ldr r7, [sp]
        add sp, #4
        bx r3

@args
@result 123
