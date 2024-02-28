        .syntax unified
        .thumb_func
fac:
        sub sp, #8
        str r4, [sp, #0]
        mov r4, lr
        str r4, [sp, #4]

        cmp r0, #0
        bne step
        movs r0, #1
        b done
step:
        mov r4, r0
        subs r0, #1
        bl fac
        muls r0, r4
done:
        ldr r4, [sp, #0]
        ldr r3, [sp, #4]
        add sp, #8
        bx r3
       
@args 10
@result 3628800
