/* thumb.ld -- Copyright (c) 2019 J. M. Spivey */

MEMORY {
    MEM (rwx) : ORIGIN = 0x00000000, LENGTH = 0x4000
}

OUTPUT_FORMAT ("elf32-littlearm")

SECTIONS {
    .mem : {
        KEEP(*(.vectors))
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
        *(.data*)
        . = ALIGN(4);
    } > MEM
}
