MEMORY
{
    EMI_SHARED      : ORIGIN = 0x24000000<<2, LENGTH = 0x04000000<<2
}

SECTIONS
{
  .data_nmgl_shared : ALIGN(0x8)
  {
    *(.data_nmgl_shared)
  } > EMI_SHARED
  .data_hal_syncro : ALIGN(0x8)
  {
    *(.data_hal_syncro);
  } > EMI_SHARED
  
}