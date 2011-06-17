/* This script controls where the different sections of a compiled program are stored in memory */
MEMORY
{
  rom (rx) : ORIGIN =  0x08000000, LENGTH = 256K   /* Flash */
  ram (rwx) : ORIGIN = 0x20000000, LENGTH =  64K   /* SRAM  */
}

SECTIONS
  {
    /* Boot memory, aliased to Flash or SRAM depending on boot pins */
    .  = 0x08000000;
    .text : {
        *(vectors)              /* Vector table */
        *(.text)                /* Program code */
        *(.rodata)              /* Read only data */
    } > rom                 /* Store program in rom (Flash) */

    /* Program data, variables that are given a value at compile time */
    .  = 0x20000000;        /* Beginning of SRAM */
    .data : {
        *(.data)                /* Data memory */
    } > ram AT > rom

    /* Statically allocated program data and uninitialized variables will be
    stored here and will be initialized to zero */
    .bss : {
        *(.bss)
    } > ram AT > rom
 }
