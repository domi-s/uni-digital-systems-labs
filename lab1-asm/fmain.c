/* common/fmain.c */
/* Copyright (c) J. M. Spivey 2018-19 */

#include "hardware.h"
#include "lib.h"

/* Pins to use for serial communication */
#define TX USB_TX
#define RX USB_RX

int txinit;              /* UART ready to transmit first char */

/* serial_init -- set up UART connection to host */
void serial_init(void)
{
    UART_ENABLE = UART_ENABLE_Disabled;
    UART_BAUDRATE = UART_BAUDRATE_9600; /* 9600 baud */
    UART_CONFIG = FIELD(UART_CONFIG_PARITY, UART_PARITY_None);
                                        /* format 8N1 */
    UART_PSELTXD = TX;                  /* choose pins */
    UART_PSELRXD = RX;
    UART_ENABLE = UART_ENABLE_Enabled;
    UART_STARTTX = 1;
    UART_STARTRX = 1;
    UART_RXDRDY = 0;
    txinit = 1;
}

/* serial_getc -- wait for input character and return it */
int serial_getc(void)
{
    while (! UART_RXDRDY) { }
    char ch = UART_RXD;
    UART_RXDRDY = 0;
    return ch;
}

/* serial_putc -- send output character */
void serial_putc(char ch)
{
    if (! txinit) {
        while (! UART_TXDRDY) { }
    }
    txinit = 0;
    UART_TXDRDY = 0;
    UART_TXD = ch;
}

/* serial_puts -- send a string character by character */
void serial_puts(const char *s)
{
    while (*s != '\0')
        serial_putc(*s++);
}

/* serial_getline -- input a line of text into buf with line editing */
void serial_getline(const char *prompt, char *buf, int nbuf)
{
    char *p = buf;

    serial_puts(prompt);

    while (1) {
        char x = serial_getc();

        switch (x) {
        case '\b':
        case 0177:
            if (p > buf) {
                p--;
                serial_puts("\b \b");
            }
            break;

        case '\r':
            *p = '\0';
            serial_puts("\r\n");
            return;

        default:
            /* Ignore other non-printing characters */
            if (x >= 040 && x < 0177 && p < &buf[nbuf]) {
                *p++ = x;
                serial_putc(x);
            }
        }
    }
}

/* print_buf -- output routine for use by printf */
void print_buf(char *buf, int n)
{
    for (int i = 0; i < n; i++) {
        char c = buf[i];
        if (c == '\n') serial_putc('\r');
        serial_putc(c);
    }
}

#define NBUF 80

/* getnum -- input a decimal or hexadecimal number */
int getnum(char *prompt)
{
    char buf[NBUF];
    serial_getline(prompt, buf, NBUF);
    if (buf[0] == '0' && (buf[1] == 'x' || buf[1] == 'X'))
        return xtou(&buf[2]);
    else
        return atoi(buf);
}

/* fmt_fixed -- format t/s as a decimal to n places */
char *fmt_fixed(unsigned t, unsigned s, int n)
{
    static char buf[32];
    char *p = &buf[32];
    unsigned v;

    /* Scale and round to n places: could round to next integer */
    v = 2 * (t % s);
    for (int d = 0; d < n; d++) v *= 10;
    v = (v+s) / (2*s);

    /* Fractional part */
    *(--p) = '\0';
    for (int d = 0; d < n; d++) {
        *(--p) = v % 10 + '0';
        v /= 10;
    }

    /* Integer part, including any carry from rounding up */
    v += t / s;
    *(--p) = '.';
    do {
        *(--p) = v % 10 + '0';
        v /= 10;
    } while (v > 0);

    return p;
}

/* easy_mod -- modulo operation for use in digits example */
unsigned easy_mod(unsigned a, unsigned b)
{
    return a % b;
}

void clock_init(void)
{
    /* Set up TIMER0 in 32 bit mode */
    TIMER0_MODE = TIMER_MODE_Timer;
    TIMER0_BITMODE = TIMER_BITMODE_32Bit;
    TIMER0_PRESCALER = 0; /* Count at 16MHz */
    TIMER0_START = 1;
}    

#define clock_start()  TIMER0_CLEAR = 1
#define clock_stop()   (TIMER0_CAPTURE[0] = 1, TIMER0_CC[0])
        
#define FUDGE 10                /* ticks of overhead for function call */
#define MULT 1                  /* clock cycles per tick */
#define FREQ 16                 /* clock frequency */

extern int func(int a, int b);

void init(void)
{
    unsigned time;

    serial_init();
    printf("\nHello micro:world!\n\n");
      
    led_init();
    clock_init();

    while (1) {
        int a, b, r;
        a = getnum("a = ");
        b = getnum("b = ");

        led_dot();
        clock_start();
        r = func(a, b);
        time = clock_stop();
        led_off();
        
        time -= FUDGE;
        printf("func(%d, %d) = %d\n", a, b, r);
        printf("func(%x, %x) = %x\n", a, b, r);
        printf("%d cycle%s, %s microsec\n\n",
               time*MULT, (time*MULT == 1 ? "" : "s"),
               fmt_fixed(time, FREQ, 3));
    }
}
