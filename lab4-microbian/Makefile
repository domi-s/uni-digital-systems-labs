# lab4-microbian/Makefile
# Copyright (c) 2018-21 J. M. Spivey

all: microbian.a examples

DRIVERS = serial.o timer.o i2c.o radio.o adc.o display.o
MICROBIAN = microbian.o mpx-m0.o $(DRIVERS) lib.o

include ../config.mk
CC = arm-none-eabi-gcc
CFLAGS = -O -g -Wall -ffreestanding $(INCLUDE)
AS = arm-none-eabi-as
AR = arm-none-eabi-ar

microbian.a: $(MICROBIAN)
	$(AR) cr $@ $^

EXAMPLES = ex-echo.hex ex-heart.hex ex-level.hex ex-race.hex \
	ex-remote.hex ex-today.hex ex-analog.hex $(LAB4EXTRA)

examples: $(EXAMPLES)

ex-level.elf: accel.o

%.elf: %.o startup.o microbian.a 
	$(CC) $(CPU) $(CFLAGS) -T $(LSCRIPT) -nostdlib \
	    $^ -lc -lgcc -o $@ -Wl,-Map,$*.map

%.o: %.c
	$(CC) $(CPU) $(CFLAGS) -c $< -o $@ 

%.o: %.s
	$(AS) $(CPU) $< -o $@

%.hex: %.elf
	arm-none-eabi-objcopy -O ihex $< $@

clean: force
	rm -f *.hex *.elf *.map *.o microbian.a

.PHONY: examples

# Don't delete intermediate files
.SECONDARY:

force:

###

$(MICROBIAN): microbian.h hardware.h
