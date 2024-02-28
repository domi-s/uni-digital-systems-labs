        .syntax unified
        .thumb_func
foo:
        movs r2, #0
        cmp r0, #0
        beq done
again:  
        adds r2, r1
        subs r0, #1
        bne again
done:   
        movs r0, r2
        bx lr
        .align 4

@args 5 7
@result 35
