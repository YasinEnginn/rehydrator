ENTRY(ResetISR)

MEMORY
{
    FLASH_IMG_HDR (RX) : ORIGIN = 0x00030000, LENGTH = 0x00000100
    FLASH         (RX) : ORIGIN = 0x00030100, LENGTH = 0x00021F00
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

    .ramVecs (NOLOAD) : ALIGN(1024)
    {
        KEEP(*(.ramVecs))
    } > SRAM

    UDMACC26XX_CONFIG_BASE = 0x20001800;

    DMA_UART0_RX_CONTROL_TABLE_ENTRY_ADDRESS  = (UDMACC26XX_CONFIG_BASE + 0x10);
    DMA_UART0_TX_CONTROL_TABLE_ENTRY_ADDRESS  = (UDMACC26XX_CONFIG_BASE + 0x20);
    DMA_SPI0_RX_CONTROL_TABLE_ENTRY_ADDRESS   = (UDMACC26XX_CONFIG_BASE + 0x30);
    DMA_SPI0_TX_CONTROL_TABLE_ENTRY_ADDRESS   = (UDMACC26XX_CONFIG_BASE + 0x40);
    DMA_UART1_RX_CONTROL_TABLE_ENTRY_ADDRESS  = (UDMACC26XX_CONFIG_BASE + 0x50);
    DMA_UART1_TX_CONTROL_TABLE_ENTRY_ADDRESS  = (UDMACC26XX_CONFIG_BASE + 0x60);
    DMA_ADC_PRI_CONTROL_TABLE_ENTRY_ADDRESS   = (UDMACC26XX_CONFIG_BASE + 0x70);
    DMA_GPT0A_PRI_CONTROL_TABLE_ENTRY_ADDRESS = (UDMACC26XX_CONFIG_BASE + 0x90);
    DMA_SPI1_RX_CONTROL_TABLE_ENTRY_ADDRESS   = (UDMACC26XX_CONFIG_BASE + 0x100);
    DMA_SPI1_TX_CONTROL_TABLE_ENTRY_ADDRESS   = (UDMACC26XX_CONFIG_BASE + 0x110);

    DMA_UART0_RX_ALT_CONTROL_TABLE_ENTRY_ADDRESS = (UDMACC26XX_CONFIG_BASE + 0x210);
    DMA_UART0_TX_ALT_CONTROL_TABLE_ENTRY_ADDRESS = (UDMACC26XX_CONFIG_BASE + 0x220);
    DMA_SPI0_RX_ALT_CONTROL_TABLE_ENTRY_ADDRESS  = (UDMACC26XX_CONFIG_BASE + 0x230);
    DMA_SPI0_TX_ALT_CONTROL_TABLE_ENTRY_ADDRESS  = (UDMACC26XX_CONFIG_BASE + 0x240);
    DMA_UART1_RX_ALT_CONTROL_TABLE_ENTRY_ADDRESS = (UDMACC26XX_CONFIG_BASE + 0x250);
    DMA_UART1_TX_ALT_CONTROL_TABLE_ENTRY_ADDRESS = (UDMACC26XX_CONFIG_BASE + 0x260);
    DMA_ADC_ALT_CONTROL_TABLE_ENTRY_ADDRESS      = (UDMACC26XX_CONFIG_BASE + 0x270);
    DMA_GPT0A_ALT_CONTROL_TABLE_ENTRY_ADDRESS    = (UDMACC26XX_CONFIG_BASE + 0x290);
    DMA_SPI1_RX_ALT_CONTROL_TABLE_ENTRY_ADDRESS  = (UDMACC26XX_CONFIG_BASE + 0x300);
    DMA_SPI1_TX_ALT_CONTROL_TABLE_ENTRY_ADDRESS  = (UDMACC26XX_CONFIG_BASE + 0x310);

    UDMACC26XX_uart0RxControlTableEntry_is_placed = 0;
    .dmaUart0RxControlTableEntry DMA_UART0_RX_CONTROL_TABLE_ENTRY_ADDRESS (NOLOAD) :
    {
        *(.dmaUart0RxControlTableEntry)
    } > SRAM

    UDMACC26XX_uart0TxControlTableEntry_is_placed = 0;
    .dmaUart0TxControlTableEntry DMA_UART0_TX_CONTROL_TABLE_ENTRY_ADDRESS (NOLOAD) :
    {
        *(.dmaUart0TxControlTableEntry)
    } > SRAM

    UDMACC26XX_dmaSpi0RxControlTableEntry_is_placed = 0;
    .dmaSpi0RxControlTableEntry DMA_SPI0_RX_CONTROL_TABLE_ENTRY_ADDRESS (NOLOAD) :
    {
        *(.dmaSpi0RxControlTableEntry)
    } > SRAM

    UDMACC26XX_dmaSpi0TxControlTableEntry_is_placed = 0;
    .dmaSpi0TxControlTableEntry DMA_SPI0_TX_CONTROL_TABLE_ENTRY_ADDRESS (NOLOAD) :
    {
        *(.dmaSpi0TxControlTableEntry)
    } > SRAM

    UDMACC26XX_uart1RxControlTableEntry_is_placed = 0;
    .dmaUart1RxControlTableEntry DMA_UART1_RX_CONTROL_TABLE_ENTRY_ADDRESS (NOLOAD) :
    {
        *(.dmaUart1RxControlTableEntry)
    } > SRAM

    UDMACC26XX_uart1TxControlTableEntry_is_placed = 0;
    .dmaUart1TxControlTableEntry DMA_UART1_TX_CONTROL_TABLE_ENTRY_ADDRESS (NOLOAD) :
    {
        *(.dmaUart1TxControlTableEntry)
    } > SRAM

    UDMACC26XX_dmaADCPriControlTableEntry_is_placed = 0;
    .dmaADCPriControlTableEntry DMA_ADC_PRI_CONTROL_TABLE_ENTRY_ADDRESS (NOLOAD) :
    {
        *(.dmaADCPriControlTableEntry)
    } > SRAM

    UDMACC26XX_dmaGPT0APriControlTableEntry_is_placed = 0;
    .dmaGPT0APriControlTableEntry DMA_GPT0A_PRI_CONTROL_TABLE_ENTRY_ADDRESS (NOLOAD) :
    {
        *(.dmaGPT0APriControlTableEntry)
    } > SRAM

    UDMACC26XX_dmaSpi1RxControlTableEntry_is_placed = 0;
    .dmaSpi1RxControlTableEntry DMA_SPI1_RX_CONTROL_TABLE_ENTRY_ADDRESS (NOLOAD) :
    {
        *(.dmaSpi1RxControlTableEntry)
    } > SRAM

    UDMACC26XX_dmaSpi1TxControlTableEntry_is_placed = 0;
    .dmaSpi1TxControlTableEntry DMA_SPI1_TX_CONTROL_TABLE_ENTRY_ADDRESS (NOLOAD) :
    {
        *(.dmaSpi1TxControlTableEntry)
    } > SRAM

    UDMACC26XX_uart0RxAltControlTableEntry_is_placed = 0;
    .dmaUart0RxAltControlTableEntry DMA_UART0_RX_ALT_CONTROL_TABLE_ENTRY_ADDRESS (NOLOAD) :
    {
        *(.dmaUart0RxAltControlTableEntry)
    } > SRAM

    UDMACC26XX_uart0TxAltControlTableEntry_is_placed = 0;
    .dmaUart0TxAltControlTableEntry DMA_UART0_TX_ALT_CONTROL_TABLE_ENTRY_ADDRESS (NOLOAD) :
    {
        *(.dmaUart0TxAltControlTableEntry)
    } > SRAM

    UDMACC26XX_dmaSpi0RxAltControlTableEntry_is_placed = 0;
    .dmaSpi0RxAltControlTableEntry DMA_SPI0_RX_ALT_CONTROL_TABLE_ENTRY_ADDRESS (NOLOAD) :
    {
        *(.dmaSpi0RxAltControlTableEntry)
    } > SRAM

    UDMACC26XX_dmaSpi0TxAltControlTableEntry_is_placed = 0;
    .dmaSpi0TxAltControlTableEntry DMA_SPI0_TX_ALT_CONTROL_TABLE_ENTRY_ADDRESS (NOLOAD) :
    {
        *(.dmaSpi0TxAltControlTableEntry)
    } > SRAM

    UDMACC26XX_uart1RxAltControlTableEntry_is_placed = 0;
    .dmaUart1RxAltControlTableEntry DMA_UART1_RX_ALT_CONTROL_TABLE_ENTRY_ADDRESS (NOLOAD) :
    {
        *(.dmaUart1RxAltControlTableEntry)
    } > SRAM

    UDMACC26XX_uart1TxAltControlTableEntry_is_placed = 0;
    .dmaUart1TxAltControlTableEntry DMA_UART1_TX_ALT_CONTROL_TABLE_ENTRY_ADDRESS (NOLOAD) :
    {
        *(.dmaUart1TxAltControlTableEntry)
    } > SRAM

    UDMACC26XX_dmaADCAltControlTableEntry_is_placed = 0;
    .dmaADCAltControlTableEntry DMA_ADC_ALT_CONTROL_TABLE_ENTRY_ADDRESS (NOLOAD) :
    {
        *(.dmaADCAltControlTableEntry)
    } > SRAM

    UDMACC26XX_dmaGPT0AAltControlTableEntry_is_placed = 0;
    .dmaGPT0AAltControlTableEntry DMA_GPT0A_ALT_CONTROL_TABLE_ENTRY_ADDRESS (NOLOAD) :
    {
        *(.dmaGPT0AAltControlTableEntry)
    } > SRAM

    UDMACC26XX_dmaSpi1RxAltControlTableEntry_is_placed = 0;
    .dmaSpi1RxAltControlTableEntry DMA_SPI1_RX_ALT_CONTROL_TABLE_ENTRY_ADDRESS (NOLOAD) :
    {
        *(.dmaSpi1RxAltControlTableEntry)
    } > SRAM

    UDMACC26XX_dmaSpi1TxAltControlTableEntry_is_placed = 0;
    .dmaSpi1TxAltControlTableEntry DMA_SPI1_TX_ALT_CONTROL_TABLE_ENTRY_ADDRESS (NOLOAD) :
    {
        *(.dmaSpi1TxAltControlTableEntry)
    } > SRAM

    .text :
    {
        _text = .;
        CREATE_OBJECT_SYMBOLS
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
        KEEP(*(.timestampPFormat))
        . = ALIGN(4);
        KEEP(*(.ctors))
        KEEP(*(.dtors))
        . = ALIGN(4);
        __init_array_start = .;
        KEEP(*(.init_array*))
        __init_array_end = .;
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

    .nvs (NOLOAD) : ALIGN(0x2000)
    {
        *(.nvs)
    } > FLASH

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
        _stack_origin = .;
        __stack = .;
        . = . + _Min_Stack_Size;
        . = ALIGN(8);
        _stack = .;
        _stack_end = .;
        __stack_end = .;
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
