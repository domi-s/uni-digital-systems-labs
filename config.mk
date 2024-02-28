# config.mk-v1
# Copyright (c) 2021 J. M. Spivey

CPU = -mcpu=cortex-m0 -mthumb
LSCRIPT = nRF51822.ld

RAMBASE = 0x20000000
RAMSIZE = 0x4000
MAGIC = 80
