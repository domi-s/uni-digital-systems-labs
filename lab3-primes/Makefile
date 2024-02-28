# lab3-primes/Makefile
# Copyright (c) 2018 J. M. Spivey

all: primes-poll.hex primes-intr.hex

include ../config.mk
CC = arm-none-eabi-gcc
CFLAGS = -O -g -Wall -ffreestanding $(INCLUDE)
AS = arm-none-eabi-as

%.elf: %.o lib.o startup.o
	$(CC) $(CPU) $(CFLAGS) -T $(LSCRIPT) -nostdlib \
	    $^ -lgcc -o $@ -Wl,-Map,$*.map 
	arm-none-eabi-size $@

%.o: %.c
	$(CC) $(CPU) $(CFLAGS) -c $< -o $@ 

%.hex: %.elf
	arm-none-eabi-objcopy -O ihex $< $@

clean:
	rm -f *.hex *.elf *.map *.o 

# Don't delete intermediate files
.SECONDARY:

