/* i2c.c */
/* Copyright (c) 2019 J. M. Spivey */

#include "microbian.h"
#include "hardware.h"
#include <stddef.h>

/* The V2 board has two I2C channels, one connected internally to the
accelerometer and magnetometer, and the other connected to the edge
connector and used for external devices.  On the V1 board, one
interface serves both purposes.  Because of this, the V2 board is
served by up to two I2C driver processes, and the V1 board by only one
at most.  Both I2C interfaces have the same register layout, and we
use some navish C language tricks to refer to both interfaces with the
same code. */

#ifdef UBIT_V1
static const int i2c_irq[] = { I2C_IRQ };
static const int i2c_scl[] = { I2C_SCL };
static const int i2c_sda[] = { I2C_SDA };

#define I2C_REG(chan, reg) I2C_##reg
#endif

#ifdef UBIT_V2
static const int i2c_irq[] = { I2C0_IRQ, I2C1_IRQ };
static const int i2c_scl[] = { I2C0_SCL, I2C1_SCL };
static const int i2c_sda[] = { I2C0_SDA, I2C1_SDA };

static unsigned const *i2c_base = { I2C0_BASE, I2C1_BASE };
#define I2C_REG(chan, reg) \
    (* (i2c_base[chan] + (&I2C0_##REG - I2C0_BASE)))
#endif

static int I2C_TASK[N_I2C];

/* i2c_wait -- wait for an expected interrupt event and detect error */
static int i2c_wait(int chan, unsigned volatile *event)
{
    receive(INTERRUPT, NULL);

    if (I2C_REG(chan, ERROR)) {
        I2C_REG(chan, ERROR) = 0; 
        clear_pending(i2c_irq[chan]);
        enable_irq(i2c_irq[chan]);
        return ERR;
    }      

    assert(*event);
    *event = 0;                                 
    clear_pending(i2c_irq[chan]);
    enable_irq(i2c_irq[chan]);
    return OK;
}

/* i2c_do_write -- send one or more bytes */
static int i2c_do_write(int chan, char *buf, int n)
{
    int status = OK;

    /* The I2C hardware makes zero-length writes impossible, because
       there is no event generated when the address has been sent. */

    for (int i = 0; i < n; i++) {
        I2C_REG(chan, TXD) = buf[i];
        status = i2c_wait(chan, &I2C_REG(chan, TXDSENT));
        if (status != OK) return status;
    }

    return OK;
}

/* i2c_stop -- signal stop condition */
static void i2c_stop(int chan)
{
    I2C_REG(chan, STOP) = 1;
    i2c_wait(chan, &I2C_REG(chan, STOPPED));
}

/* i2c_task -- driver process for I2C hardware */
static void i2c_task(int chan)
{
    int scl = i2c_scl[chan], sda = i2c_sda[chan], irq = i2c_irq[chan];
    message m;
    int client, addr, n1, n2, status, error = 0;
    char *buf1, *buf2;

    /* Configure pins -- thanks to friends at University of Cantabria */
    SET_FIELD(GPIO_PINCNF[scl], GPIO_PINCNF_DRIVE, GPIO_DRIVE_S0D1);
    SET_FIELD(GPIO_PINCNF[sda], GPIO_PINCNF_DRIVE, GPIO_DRIVE_S0D1);

    /* Configure I2C hardware */
    I2C_REG(chan, PSELSCL) = scl;
    I2C_REG(chan, PSELSDA) = sda;
    I2C_REG(chan, FREQUENCY) = I2C_FREQUENCY_100kHz;
    I2C_REG(chan, ENABLE) = I2C_ENABLE_Enabled;

    /* Enable interrupts */
    I2C_REG(chan, INTEN) = BIT(I2C_INT_RXDREADY) | BIT(I2C_INT_TXDSENT)
        | BIT(I2C_INT_STOPPED) | BIT(I2C_INT_ERROR);
    connect(irq);
    enable_irq(irq);

    while (1) {
        receive(ANY, &m);
        client = m.sender;
        addr = m.byte1;        /* Address [0..127] without R/W flag */
        n1 = m.byte2;          /* Number of bytes in command */
        n2 = m.byte3;          /* Number of bytes to transfer (R/W) */
        buf1 = m.ptr2;        /* Buffer for command */
        buf2 = m.ptr3;        /* Buffer for transfer */

        switch (m.type) {
        case READ:
            I2C_ADDRESS = addr;
            status = OK;
             
            if (n1 > 0) {
                /* Write followed by read, with repeated start */
                I2C_REG(chan, STARTTX) = 1;
                status = i2c_do_write(chan, buf1, n1);
            }

            /* The hardware reference manual is wrong in several ways,
               but the following code (based on timing diagrams in the
               reference manual) works reliably. */

            if (status == OK) {
                for (int i = 0; i < n2; i++) {
                    /* On all but the last byte, use SUSPEND to send
                       an ACK after receiving the byte.  Use STOP to
                       send a NACK at the end. */
                    if (i < n2-1)
                        I2C_REG(chan, SHORTS) = BIT(I2C_BB_SUSPEND);
                    else
                        I2C_REG(chan, SHORTS) = BIT(I2C_BB_STOP);
        
                    /* Start the first byte with STARTTX, and the rest
                       with RESUME following the SUSPEND. */
                    if (i == 0)
                        I2C_REG(chan, STARTRX) = 1;
                    else
                        I2C_REG(chan, RESUME) = 1;
        
                    status = i2c_wait(chan, &I2C_REG(chan, RXDREADY));
                    if (status != OK) break;
                    buf2[i] = I2C_REG(chan, RXD);
                }
            }
            
            if (status == OK)
                i2c_wait(chan, &I2C_REG(chan, STOPPED));

            if (status != OK) {
                i2c_stop(chan);
                error = I2C_REG(chan, ERRORSRC);
                I2C_REG(chan, ERRORSRC) = I2C_ERRORSRC_All;
            }

            I2C_SHORTS = 0;
            m.type = REPLY;
            m.int1 = status;
            m.int2 = error;
            send(client, &m);
            break;

        case WRITE:
            I2C_REG(chan, ADDRESS) = addr;
            status = OK;

            /* A single write transaction */
            I2C_REG(chan, STARTTX) = 1;
            if (n1 > 0)
                status = i2c_do_write(chan, buf1, n1);
            if (status == OK && n2 > 0)
                status = i2c_do_write(chan, buf2, n2);
            i2c_stop(chan);

            if (status != OK) {
                error = I2C_REG(chan, ERRORSRC);
                I2C_REG(chan, ERRORSRC) = I2C_ERRORSRC_All;
            }
               
            m.type = REPLY;
            m.int1 = status;
            m.int2 = error;
            send(client, &m);
            break;

        default:
            badmesg(m.type);
        }
    }
}

/* i2c_init -- start I2C driver process */
void i2c_init(int chan)
{
    if (I2C_TASK[chan] == 0)
        I2C_TASK[chan] = start("I2C", i2c_task, chan, 256);
}

/* i2c_xfer -- i2c transaction with command write then data read or write */
int i2c_xfer(int chan, int kind, int addr,
             byte *buf1, int n1, byte *buf2, int n2) {
    message m;
    m.type = kind;
    m.byte1 = addr;
    m.byte2 = n1;
    m.byte3 = n2;
    m.ptr2 = buf1;
    m.ptr3 = buf2;
    sendrec(I2C_TASK[chan], &m);
    return m.int1;
}

/* i2c_probe -- try to access an I2C device */
int i2c_probe(int chan, int addr)
{
    byte buf = 0;
    return i2c_xfer(chan, WRITE, addr, &buf, 1, NULL, 0);
}
     
/* i2c_read_bytes -- send command and read multi-byte result */
void i2c_read_bytes(int chan, int addr, int cmd, byte *buf2, int n2)
{
    byte buf1 = cmd;
    int status = i2c_xfer(chan, READ, addr, &buf1, 1, buf2, n2);
    assert(status == OK);
}

/* i2c_read_reg -- send command and read one byte */
int i2c_read_reg(int chan, int addr, int cmd)
{
    byte buf;
    i2c_read_bytes(chan, addr, cmd, &buf, 1);
    return buf;
}

/* i2c_write_bytes -- send command and write multi-byte data */
void i2c_write_bytes(int chan, int addr, int cmd, byte *buf2, int n2)
{
    byte buf1 = cmd;
    int status = i2c_xfer(chan, WRITE, addr, &buf1, 1, buf2, n2);
    assert(status == OK);
}

/* i2c_write_reg -- send command and write data */
void i2c_write_reg(int chan, int addr, int cmd, int val)
{
    byte buf = val;
    i2c_write_bytes(chan, addr, cmd, &buf, 1);
}
