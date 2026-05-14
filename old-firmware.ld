ENTRY(ResetISR)

MEMORY
{
    FLASH_IMG_HDR (RX) : ORIGIN = 0x00000000, LENGTH = 0x00000100
    FLASH         (RX) : ORIGIN = 0x00000100, LENGTH = 0x0002FF00
    SRAM          (RWX): ORIGIN = 0x20000000, LENGTH = 0x00014000
    GPRAM         (RWX): ORIGIN = 0x11000000, LENGTH = 0x00002000
}

_estack = ORIGIN(SRAM) + LENGTH(SRAM);
_Min_Heap_Size = 0;
_Min_Stack_Size = 0x600;

SECTIONS
{
    .image_header :
    {
        . = ALIGN(4);
        KEEP(*(.image_header))
        KEEP(*(.oad_image_header))
        . = ALIGN(4);
    } > FLASH_IMG_HDR

    ASSERT(SIZEOF(.image_header) <= LENGTH(FLASH_IMG_HDR),
           "OAD image header exceeds reserved 0x100 bytes")

    .resetVecs :
    {
        . = ALIGN(4);
        KEEP(*(.resetVecs))
        KEEP(*(.vectors))
    } > FLASH

    .text :
    {
        _text = .;
        CREATE_OBJECT_SYMBOLS
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(4);
        KEEP(*(.ctors))
        KEEP(*(.dtors))
        KEEP(*(.init_array*))
        *(.init)
        *(.fini*)
        _etext = .;
    } > FLASH = 0

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > FLASH

    .ramVecs (NOLOAD) : ALIGN(1024)
    {
        KEEP(*(.ramVecs))
    } > SRAM

    .data : ALIGN(4)
    {
        _data = .;
        __data_start__ = .;
        *(vtable)
        *(.data)
        *(.data.*)
        . = ALIGN(4);
        _edata = .;
        __data_end__ = .;
    } > SRAM AT > FLASH
    _ldata = LOADADDR(.data);
    __data_load__ = LOADADDR(.data);

    .bss (NOLOAD) : ALIGN(4)
    {
        _bss = .;
        __bss_start__ = .;
        *(.shbss)
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = .;
    } > SRAM

    .heap (NOLOAD) : ALIGN(4)
    {
        PROVIDE(__heap_start__ = .);
        PROVIDE(_heap = __heap_start__);
        PROVIDE(end = __heap_start__);
        PROVIDE(_end = __heap_start__);
        . = . + _Min_Heap_Size;
        PROVIDE(__heap_end__ = .);
        PROVIDE(_eheap = ORIGIN(SRAM) + LENGTH(SRAM));
    } > SRAM

    .stack (NOLOAD) :
    {
        . = ALIGN(8);
        . = . + _Min_Stack_Size;
        . = ALIGN(8);
        _stack = .;
    } > SRAM

    .gpram (NOLOAD) :
    {
    } > GPRAM

    /DISCARD/ :
    {
        *(.ccfg)
        *(.ccfg.*)
    }
}
