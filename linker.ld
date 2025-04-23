MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 512K
  RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS
{
  .text : {
    KEEP(*(.isr_vector))
    *(.text*)
    *(.rodata*)
    . = ALIGN(4);
    *(.ARM.exidx* .ARM.exidx.*)
    _etext = .;
} > FLASH


  .data : AT (_etext) {
    _data = .;
    *(.data*)
    _edata = .;
  } > RAM

    .bss : {
    _bss = .;
    *(.bss*)
    *(COMMON)
    _ebss = .;
    _end = .;
    PROVIDE(end = .);
  } > RAM

  .stack (COPY) : {
    . = ALIGN(8);
    _stack_start = .;
    . = . + 1024;
    _stack_end = .;
  } > RAM
}
