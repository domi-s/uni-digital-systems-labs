/* @DIR x4000@/adc.c */
/* Copyright (c) 2021 J. M. Spivey */

/* Device driver for analog to digital converter on micro:bit V1 or V2 */

#include "microbian.h"
#include "hardware.h"

static int ADC_TASK;            /* PID of driver process */

/* adc_task -- device driver */
static void adc_task(int dummy)
{
    int client, chan;
    short result;
    message m;

    /* Initialise the ADC: 10 bit resolution,
       compare 1/3 of the input with 1/3 of Vdd. */
    ADC_CONFIG = FIELD(ADC_CONFIG_RES, ADC_RES_10bit)
        | FIELD(ADC_CONFIG_INPSEL, ADC_INPSEL_AIn_1_3)
        | FIELD(ADC_CONFIG_REFSEL, ADC_REFSEL_Vdd_1_3);
    ADC_INTEN = BIT(ADC_INT_END);

    connect(ADC_IRQ);
    enable_irq(ADC_IRQ);
    
    while (1) {
        /* Wait for a request */
        receive(REQUEST, &m);
        client = m.sender;
        chan = m.int1;

        /* Carry out an ADC acquisition */
        SET_FIELD(ADC_CONFIG, ADC_CONFIG_PSEL, BIT(chan));
        ADC_ENABLE = 1;
        ADC_START = 1;
        receive(INTERRUPT, NULL);
        assert(ADC_END);
        result = ADC_RESULT;
        ADC_END = 0;
        ADC_ENABLE = 0;
        
        clear_pending(ADC_IRQ);
        enable_irq(ADC_IRQ);

        /* Reply to the client */
        send_int(client, REPLY, result);
    }
}

/* chantab -- translate pin numbers to ADC channels */
static const int chantab[] = {
    PAD0, 4, PAD1, 3, PAD2, 2, PAD3, 5, PAD4, 6, PAD10, 7,
    0
};

/* adc_reading -- get ADC reading on specfied pin */
int adc_reading(int pin)
{
    int i, chan = -1;
    message m;

    for (i = 0; chantab[i] != 0; i += 2) {
        if (chantab[i] == pin) {
            chan = chantab[i+1];
            break;
        }
    }

    if (chan < 0)
        panic("Can't use pin %d for ADC", pin);

    m.type = REQUEST;
    m.int1 = chan;
    sendrec(ADC_TASK, &m);
    return m.int1;
}

/* adc_init -- start ADC driver */
void adc_init(void)
{
    ADC_TASK = start("ADC", adc_task, 0, 256);
}
