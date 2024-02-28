// lab4/ex-analog.c
// Copyright (c) 2021 J. M. Spivey

#include "microbian.h"
#include "lib.h"

void user_task(int n) {
    // Reads from uBit pin 2 = chip pin P0.01 = AIN2

    printf("ADC example\n");

    while (1) {
        printf("%d\n", adc_reading(2));
        timer_delay(500);
    }
}

void init(void) {
    timer_init();
    serial_init();
    adc_init();
    start("User", user_task, 0, STACK);
}
