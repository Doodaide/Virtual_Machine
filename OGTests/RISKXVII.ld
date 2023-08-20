INST_SIZE = 1024;
DATA_START = INST_SIZE;
DATA_SIZE = 1024;

ADDR_END = DATA_START + DATA_SIZE;

MEMORY {
    imem (x): org = 0x00, len = INST_SIZE /* SIZE OF INSTRUCTION CACHE */
    dmem (rw): org = DATA_START, len = DATA_SIZE /* SIZE OF DATA CACHE */
}

SECTIONS {
    .text : {
        . = ALIGN(4);
        *(.text)
    } > imem

    
    .data : {
        . = ALIGN(4);
        *(.rodata)

        . = ALIGN(4);
        *(.bss)

        . = ALIGN(4);
        *(.sbss)

        . = ALIGN(4);
        *(.data)

        . = ALIGN(4);
        *(.sdata)

        . = ALIGN(4);
        . = DATA_SIZE;
        __stack_end = . - 1;

    } > dmem

}