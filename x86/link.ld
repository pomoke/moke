ENTRY(loader)                /* the name of the entry label */

SECTIONS {
    . = 0x00100000;          /* the code should be loaded at 1 MB */

    .text.early ALIGN (0x1000):
    {
	    *(.text.early)
    }

    .bss.early ALIGN (0x1000):
    {
	    *(.bss.early)
    }
    . = 0xc0200000;
    .text ALIGN (0x1000) : AT(ADDR(.text)-0xc0000000)
    {
        *(.text)             /* all text sections from all files */
    }

    .rodata ALIGN (0x1000) : AT(ADDR(.rodata)-0xc0000000)
    {
        *(.rodata*)          /* all read-only data sections from all files */
    }

    .data ALIGN (0x1000) :  AT(ADDR(.data)-0xc0000000)
    {
        *(.data)             /* all data sections from all files */
    }


    .bss ALIGN (0x1000) :   AT(ADDR(.bss)-0xc0000000)
    {
        *(COMMON)            /* all COMMON sections from all files */
        *(.bss)              /* all bss sections from all files */
    }
    
    
}
