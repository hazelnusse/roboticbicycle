MEMORY
{
    sram  (RWX) : ORIGIN = 0x20000000, LENGTH = 64k
    flash  (RX) : ORIGIN = 0x08000000, LENGTH = 256k-2k
    eemul (RWX) : ORIGIN = 0x08000000+256k-2k, LENGTH = 2k
}

SECTIONS
{
  .text :
  {
    . = ALIGN(4);
    _text = .;
    /* PROVIDE(stext = .); */
    KEEP(*(.isr_vector))
    /*KEEP(*(.init))*/
    *(.text .text.*)
    *(.rodata .rodata.*)
    /* *(.gnu.linkonce.t.*)
    *(.glue_7)
    *(.glue_7t)
    *(.gcc_except_table)
    *(.gnu.linkonce.r.*) */
    . = ALIGN(4);
    _etext = .;
    _sidata = _etext;
    /* PROVIDE(etext = .);
    _fini = . ;
    *(.fini) */
  } > flash

  .data : AT (_etext)
  {
    . = ALIGN(4);
    _sdata = .;
    /**(.ramfunc .ramfunc.* .fastrun .fastrun.*)*/
    *(.data .data.*)
    /* *(.gnu.linkonce.d.*) doesn't occur unless c++ rtti is used */
    . = ALIGN(4);
    _edata = .;
  } > sram

  /* This is for exception unwinding information.  We aren't using exceptions,
   * so leave it out
  .ARM.extab :
  {
    *(.ARM.extab*)
  } > sram

  __exidx_start = .;
  .ARM.exidx :
  {
    *(.ARM.exidx*)
  } >sram
  __exidx_end = .;
  */

  .bss (NOLOAD) :
  {
    . = ALIGN(4);
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;
    *(.bss .bss.*)
    /* *(.gnu.linkonce.b.*) this section doesn't get used unless c++ rtti is
     * used */
    /* *(COMMON) */
    . = ALIGN(4);
    _ebss = .;
    /*_end = .;*/
    /*__end = .; */
  } > sram

  /*
  /DISCARD/ : { *(.eh_frame) }
  */

  /*end = .;*/
  PROVIDE( _estack = 0x20010000 );
}
