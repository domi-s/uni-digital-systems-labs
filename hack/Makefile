# hack/Makefile

all: total.hex attack squirt

include ../config.mk
CC = arm-none-eabi-gcc
CFLAGS = -O -g -Wall -ffreestanding $(INCLUDE)
AS = arm-none-eabi-as

total.elf: total.o lib.o startup.o
	$(CC) $(CPU) $(CFLAGS) -T $(LSCRIPT) -nostdlib \
	    $^ -lgcc -o $@ -Wl,-Map,total.map 
	arm-none-eabi-size $@

attack: attack.o
	arm-none-eabi-objcopy -O binary attack.o attack.bin
	hexdump -v -e '/4 "%d\n"' attack.bin >attack

squirt: squirt.c
	gcc $< -o $@

RAMBASE = 0x20000000
RAMSIZE = 0x4000
MAGIC = 80

attack.s0: attack.in total.elf Makefile ../config.mk
	addr1=$$(arm-none-eabi-nm total.elf | sed -n '/T printf/s/ .*//p'); \
	    addr2=$$(printf "%x" $$(($(RAMBASE) + $(RAMSIZE) - $(MAGIC)))); \
	    sed -e "s/PRINTF/0x$$addr1/" -e "s/FRAME/0x$$addr2/" $< >$@
	cmp attack.s $@

%.o: %.c
	$(CC) $(CPU) $(CFLAGS) -c $< -o $@ 

%.o: %.s
	$(AS) $(CPU) $< -o $@

%.hex: %.elf
	arm-none-eabi-objcopy -O ihex $< $@

%.bin: %.elf
	arm-none-eabi-objcopy -O binary $< $@

clean:
	rm -f *.hex *.bin *.elf *.map *.o attack.s0 attack squirt

# Don't delete intermediate files
.SECONDARY:
