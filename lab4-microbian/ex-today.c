// lab4-phos/today.c
// Copyright (c) 2018 J. M. Spivey

#include "microbian.h"
#include "lib.h"
#include <string.h>

void put_string(char *s) {
    for (char *p = s; *p != '\0'; p++)
        serial_putc(*p);
}

char *slogan[] = {
    "no deal is better than a bad deal\n",
    "BREXIT MEANS BREXIT!\n"
};

void process(int n) {
    while (1) {
        put_string(slogan[n]);
    }
}

void init(void) {
    serial_init();
    start("May", process, 0, STACK);
    start("Farage", process, 1, STACK);
}
