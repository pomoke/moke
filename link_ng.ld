ENTRY(loader)                /* the name of the entry label */
/*OUTPUT_ARCH(i686)*/

SECTIONS {
    . = 0x00100000;          /* the code should be loaded at 1 MB */

    PROVIDE(_early_text = .);
    .text.early ALIGN (0x1000):
    {
	    *(.text.early)
    }
    PROVIDE(_end_early_text = .);
    PROVIDE(_bss_early = .);
    .bss.early ALIGN (0x1000):
    {
	    *(.bss.early)
    }
    PROVIDE(_end_bss_early = .);
    . += 0xc0000000;

    PROVIDE(_text = .);
    .text ALIGN (0x1000) : AT(ADDR(.text)-0xc0000000)
    {
        *(.text)             /* all text sections from all files */
    }
    PROVIDE(_end_text = .);

    PROVIDE(_rodata = .);
    .rodata ALIGN (0x1000) : AT(ADDR(.rodata)-0xc0000000)
    {
        *(.rodata*)          /* all read-only data sections from all files */
    }
    PROVIDE(_end_rodata = .);

    PROVIDE(_data = .);
    .data ALIGN (0x1000) :  AT(ADDR(.data)-0xc0000000)
    {
        *(.data)             /* all data sections from all files */
    }
    PROVIDE(_end_data = .);

    PROVIDE(_bss = .);
    .bss ALIGN (0x1000) :   AT(ADDR(.bss)-0xc0000000)
    {
        *(COMMON)            /* all COMMON sections from all files */
        *(.bss)              /* all bss sections from all files */
    }

    PROVIDE(_end_bss = .);

    PROVIDE(_end = .);
    
    
}
