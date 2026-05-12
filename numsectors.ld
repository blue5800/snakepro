ENTRY(_start)
SECTIONS
{
    . = 0x7C00;
    .boot : {
        *(.boot)
    }

    . = 0x7E00;
    _stage2_start = .;
    .text : {
        *(.text)
    }
    .data : {
        *(.data)
    }
    .rodata : {
        *(.rodata)
    }
    .bss : {
        *(.bss)
        *(COMMON)
    }
    . = ALIGN(512);
    _stage2_end = .;

    SECTORS_TO_LOAD = (_stage2_end - _stage2_start + 511) / 512;
}
