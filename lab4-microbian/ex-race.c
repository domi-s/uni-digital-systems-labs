// lab4-microbian/ex-race.c
// Copyright (c) 2018 J. M. Spivey

#include "microbian.h"
#include "hardware.h"
#include "lib.h"

static volatile int r = 0;

void proc1(int n) {
    for (int i = 0; i < 10; i++)
        printf("r = %d\n", r);
}

void proc2(int n) {
    while (r < 100000)
        r++;

    for (int i = 0; i < 2000000; i++) {
        nop(); nop(); nop();
    }

    dump();
}

void init(void) {
    serial_init();
    start("Proc1", proc1, 0, STACK);
    start("Proc2", proc2, 0, STACK);
}
