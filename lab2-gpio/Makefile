# lab2-gpio/Makefile
# Copyright (c) 2018 J. M. Spivey

all: heart.hex

include ../config.mk
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
CFLAGS = -O -g -Wall -ffreestanding $(INCLUDE)

blinky.elf: blinky.o
	arm-none-eabi-ld -T $(LSCRIPT) $^ -o $@ -Map blinky.map
	arm-none-eabi-size $@

%.elf: %.o startup.o
	$(CC) $(CPU) $(CFLAGS) -T $(LSCRIPT) -nostdlib \
	    $^ -lgcc -lc -o $@ -Wl,-Map,$*.map 
	arm-none-eabi-size $@

%.o: %.c
	$(CC) $(CPU) $(CFLAGS) -c $< -o $@ 

%.o: %.s
	$(AS) $(CPU) $< -o $@

%.hex: %.elf
	arm-none-eabi-objcopy -O ihex $< $@

clean:
	rm -f *.hex *.elf *.map *.o 

# Don't delete intermediate files
.SECONDARY:
