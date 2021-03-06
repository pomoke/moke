ENTRY(loader)                /* the name of the entry label */
OUTPUT_ARCH(i486:x86-64)
SECTIONS {
    . = 0x00100000;          /* the code should be loaded at 1 MB */
    PROVIDE(_at=.);
    PROVIDE(_early_text=.);

    .text.early ALIGN (0x1000):
    {
	    *(.text.early)
    }
    PROVIDE(_end_early_text=.);
    PROVIDE(_text=.);
    .text ALIGN (0x1000) :   /* align at 4 KB */
    {
        *(.text)             /* all text sections from all files */
    }
    PROVIDE(_end_text=.);

    PROVIDE(_rodata=.);
    .rodata ALIGN (0x1000) : /* align at 4 KB */
    {
        *(.rodata*)          /* all read-only data sections from all files */
    }
    PROVIDE(_end_rodata=.);

    PROVIDE(_data=.);
    .data ALIGN (0x1000) :   /* align at 4 KB */
    {
        *(.data)             /* all data sections from all files */
    }
    PROVIDE(_end_data=.);

    PROVIDE(_bss=.);
    .bss ALIGN (0x1000) :    /* align at 4 KB */
    {
        *(COMMON)            /* all COMMON sections from all files */
        *(.bss)              /* all bss sections from all files */
    }
    PROVIDE(_end_bss=.);
    PROVIDE(_end=.);
    
}
