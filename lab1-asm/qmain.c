// lab1-asm/test.c
// Copyright (c) 2018-19 J. M. Spivey

#include <stdio.h>
#include <stdlib.h>

unsigned func(int, int);

int getnum(char *s)
{
    if (s == NULL)
        return 0;
    else if (s[0] == '-')
        return strtol(s, NULL, 0);
    else
        return strtoul(s, NULL, 0);
}

int main(int argc, char **argv)
{
    int a = getnum(argv[1]);
    int b = getnum(argv[2]);
    int c = func(a, b);
    printf("func(%d, %d) = %u\n", a, b, c);
    printf("func(%#x, %#x) = %#x\n", a, b, c);
    return 0;
}
