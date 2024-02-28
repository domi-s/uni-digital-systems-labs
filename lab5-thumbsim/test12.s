@ Check that branch instructions don't write the flags.

        .syntax unified
        .text

foo:
        mov r7, lr              @ Save return address
        movs r6, #10

        ldr r0, =-400
        bl sign
        movs r5, r0

        movs r0, #0
        bl sign
        muls r5, r6
        adds r5, r0

        ldr r0, =546
        bl sign
        muls r5, r6
        adds r5, r0

        movs r0, r5
        bx r7
        
sign:
        cmp r0, #0
        blt neg
        bgt pos
        movs r0, #2
        b done

neg:
        movs r0, #1
        b done

pos:
        movs r0, #3
done:
        bx lr

@args
@result 123
