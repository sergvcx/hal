MEMORY
{
  C1_IMU0 : ORIGIN = 0x00000200<<2, LENGTH = 0x00007E00<<2
  C1_IMU1 : ORIGIN = 0x00008000<<2, LENGTH = 0x00008000<<2
  C1_IMU2 : ORIGIN = 0x00010000<<2, LENGTH = 0x00008000<<2
  C1_IMU3 : ORIGIN = 0x00018000<<2, LENGTH = 0x00008000<<2
  SHMEM1  : ORIGIN = 0x000B0000<<2, LENGTH = 0x00010000<<2
  EMI1    : ORIGIN = 0x23000000<<2, LENGTH = 0x01000000<<2
}

SECTIONS
{
  
  .init : ALIGN(0x8)  
  {
    *(.init);
  } > C1_IMU0
  .fini : ALIGN(0x8)  
  {
    *(.fini);
  } > C1_IMU0
  .nobits : ALIGN(0x8)
  {
    *(.nobits);
  } > C1_IMU0
  .data : ALIGN(0x8)
  {
    *(.data);
  } > EMI1
  .bss : ALIGN(0x8)
  {
    *(.bss);
  } > EMI1
  .stack : ALIGN(0x8)
  {
    *(.stack);
  } > C1_IMU0

  .data_demo3d : ALIGN(0x8)
  {
    *(.data_demo3d);
  } > C1_IMU0
  .data_asm : ALIGN(0x8)
  {
    *(.data_asm);
  } > C1_IMU0


  .text_imu0 : ALIGN(0x8)
  {
    *(.text_nmplv);
    *(.text_nmvcore);
    /* *libgcc.a:*(.text); */
    /* *(.text_nmprofiler); */
  } > C1_IMU0

  .text : ALIGN(0x8)
  {
    *(.text);
  } > EMI1

  .data_imu0 : ALIGN(0x8)
  {
    *(.data_imu0);
    *(.data_nmplv_G)
    *(.data_nmplv_L)
    *(.data_nmplv)
  } > C1_IMU0
  .bss.data_imu0 (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_imu0);
    *(.bss.data_nmplv_G)
    *(.bss.data_nmplv_L)
    *(.bss.data_nmplv)
  } > C1_IMU0
  .data_imu1 : ALIGN(0x8)
  {
    *(.data_imu1);
  } > C1_IMU1
  .bss.data_imu1 (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_imu1);
  } > C1_IMU1

  .data_imu2 : ALIGN(0x8)
  {
    *(.data_imu2);
  } > C1_IMU2
  .bss.data_imu2 (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_imu2);
  } > C1_IMU2

  .data_imu3 : ALIGN(0x8)
  {
    *(.data_imu3);
  } > C1_IMU3
  .bss.data_imu3 (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_imu3);
  } > C1_IMU3

  .data_shmem1 : ALIGN(0x8)
  {
    *(.data_shmem0);
  } > SHMEM1
  .bss.data_shmem1 (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_shmem1);
  } > SHMEM1

  .rpc_services : ALIGN(0x8)
  {
    KEEP(*libnm6407_io_nmc.a:*(.rpc_services));
  } > EMI1

  .rpc_services.bss (NOLOAD) : ALIGN(0x8)
  {
    *libnm6407_io_nmc.a:*(.rpc_services.bss);
  } > EMI1




  .heap : ALIGN(0x8)
  {
	*(.heap);
  } > EMI1
  .heap1 : ALIGN(0x8)
  {
    *(.heap1);
  } > EMI1
  .heap2 : ALIGN(0x8)
  {
    *(.heap2);
	. = ORIGIN(C1_IMU2) + LENGTH(C1_IMU2);
    __HEAP_2_END = .;
  } > C1_IMU2
  .heap3 : ALIGN(0x8)
  {
    *(.heap3);
	. = ORIGIN(C1_IMU3) + LENGTH(C1_IMU3);
    __HEAP_3_END = .;
  } > C1_IMU3
  .heap9 : ALIGN(0x8)
  {
    *(.heap9);
	. = ORIGIN(SHMEM1) + LENGTH(SHMEM1);
    __HEAP_9_END = .;
  } > SHMEM1
  .heap11 : ALIGN(0x8)
  {
    *(.heap11);
	. = ORIGIN(EMI1) + LENGTH(EMI1);
    __HEAP_11_END = .;
  } > EMI1
  
}