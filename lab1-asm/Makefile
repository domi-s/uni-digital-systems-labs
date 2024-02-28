# lab1-asm/Makefile
# Copyright (c) 2018 J. M. Spivey

# Uncomment the next line to use gcc for linking
#GCCLINK = 1

all: func.hex

include ../config.mk
CC = arm-none-eabi-gcc
CFLAGS = -O -g -Wall -ffreestanding $(INCLUDE)
AS = arm-none-eabi-as
LD = arm-none-eabi-ld

# The easy thing to do is invoke gcc for linking.  But it's nice to invoke
# ld directly at first, to show who is really doing the work.  For this,
# we need to find the location of the library libgcc.a in a semi-portable
# way.

ifdef GCCLINK

# In case of emergency, use the following command for linking.
%.elf: %.o fmain.o lib.o startup.o
	$(CC) $(CPU) $(CFLAGS) -T $(LSCRIPT) -nostdlib \
	    $^ -lgcc -o $@ -Wl,-Map,$*.map 
	arm-none-eabi-size $@

else

# First find the initial element of the gcc library search path.
GCCLIB := $(shell $(CC) --print-search-dirs \
		| sed -n '/^libraries: =\([^:]*\)\/:.*/s//\1/p')

# Then try a couple of likely-sounding subdirectories for the library.
# Trickily we use the 'wildcard' function here to test for the existence of
# a file, and take the first result with the 'word' function.
LIBGCC := $(word 1,$(wildcard \
		$(GCCLIB)/thumb/v6-m/libgcc.a \
		$(GCCLIB)/armv6-m/libgcc.a \
		$(GCCLIB)/thumb/v6-m/nofp/libgcc.a) UNKNOWN)

# A linking command that invokes ld explicitly instead of using the gcc
# command as a wrapper.
%.elf: fmain.o %.o lib.o startup.o
	$(LD) $^ $(LIBGCC) -o $@ -Map $*.map -T $(LSCRIPT)
	arm-none-eabi-size $@

endif

%.o: %.c
	$(CC) $(CPU) $(CFLAGS) -c $< -o $@ 

# Note that the assembler uses Cortex-M0 even on V2: change it if you like.
%.o: %.s
	$(AS) -mcpu=cortex-m0 -mthumb $< -o $@

%.hex: %.elf
	arm-none-eabi-objcopy -O ihex $< $@

# Nuke the default rules for building executables
SORRY = echo "Please say 'make $@.hex' to compile the '$@' program"
%: %.s; @$(SORRY)
%: %.o; @$(SORRY)

clean:
	rm -f *.hex *.elf *.map *.o 

# Don't delete intermediate files
.SECONDARY:

# Alternative rules to build a binary for use with qemu-arm

QEMU-CPU = -march=armv6

q-%.elf: %-q.o qmain.o
	$(QEMU-GCC) $(QEMU-CPU) $^ -static -o $@

%-q.o: %.s
	$(QEMU-AS) $(QEMU-CPU) -mthumb $< -o $@

qmain.o: qmain.c
	$(QEMU-GCC) $(QEMU-CPU) -c $< -o $@

QEMU-GCC = arm-linux-gnueabihf-gcc -marm
QEMU-AS = arm-linux-gnueabihf-as
