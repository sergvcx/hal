MEMORY
{
  C0_IMU0 : ORIGIN = 0x00000200<<2, LENGTH = 0x00007E00<<2
  C0_IMU1 : ORIGIN = 0x00008000<<2, LENGTH = 0x00008000<<2
  C0_IMU2 : ORIGIN = 0x00010000<<2, LENGTH = 0x00008000<<2
  C0_IMU3 : ORIGIN = 0x00018000<<2, LENGTH = 0x00008000<<2
  C0_IMU4 : ORIGIN = 0x00020000<<2, LENGTH = 0x00008000<<2
  C0_IMU5 : ORIGIN = 0x00028000<<2, LENGTH = 0x00008000<<2
  C0_IMU6 : ORIGIN = 0x00030000<<2, LENGTH = 0x00008000<<2
  C0_IMU7 : ORIGIN = 0x00038000<<2, LENGTH = 0x00008000<<2
  SHMEM0  : ORIGIN = 0x000A8000<<2, LENGTH = 0x00008000<<2
  EMI0    : ORIGIN = 0x20000000<<2, LENGTH = 0x03000000<<2
}

SECTIONS
{
  .text : ALIGN(0x8)
  {
    *(.text);
  } > EMI0
  .init : ALIGN(0x8)
  {
    *(.init);
  } > C0_IMU0
  .fini : ALIGN(0x8)
  {
    *(.fini);
  } > C0_IMU0
  .nobits : ALIGN(0x8)
  {
    *(.nobits);
  } > C0_IMU0
  .data : ALIGN(0x8)
  {
    *(.data);
    *(.data_DDR);    
  } > EMI0
  .bss : ALIGN(0x8)
  {
    *(.bss);
  } > SHMEM0
  .stack : ALIGN(0x8)
  {
    *(.stack);
  } > C0_IMU1



  .text_imu0 : ALIGN(0x8)
  {
    *(.text_nmblas);
    *(.text_nmplv);
  } > C0_IMU0

  .data_asm : ALIGN(0x8)
  {
    *(.data_asm);
  } > C0_IMU0


  .data_imu0 : ALIGN(0x8)
  {
    *(.data_imu0);
    *(.data_nmplv);
     *(.data_demo3d);
  } > C0_IMU0
  .bss.data_imu0 (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_demo3d)
    *(.bss.data_imu0);
  } > C0_IMU0
  .data_imu1 : ALIGN(0x8)
  {
    *(.data_imu1);
  } > C0_IMU1
  .bss.data_imu1 (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_imu1);
  } > C0_IMU1

  .data_imu2 : ALIGN(0x8)
  {
    *(.data_imu2);
  } > C0_IMU2
  .bss.data_imu2 (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_imu2);
  } > C0_IMU2

  .data_imu3 : ALIGN(0x8)
  {
    *(.data_imu3);
  } > C0_IMU3
  .bss.data_imu3 (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_imu3);
  } > C0_IMU3

  .data_imu4 : ALIGN(0x8)
  {
    *(.data_imu4);
  } > C0_IMU4
  .bss.data_imu4 (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_imu4);
  } > C0_IMU4

  .data_imu5 : ALIGN(0x8)
  {
    *(.data_imu5);
  } > C0_IMU5
  .bss.data_imu5 (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_imu5);
  } > C0_IMU5

  .data_imu6 : ALIGN(0x8)
  {
    *(.data_imu6);
  } > C0_IMU6
  .bss.data_imu6 (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_imu6);
  } > C0_IMU6

  .data_imu7 : ALIGN(0x8)
  {
    *(.data_imu7);
  } > C0_IMU7
  .bss.data_imu7 (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_imu7);
  } > C0_IMU7

  .data_shmem0 : ALIGN(0x8)
  {
    *(.data_shmem0);
  } > SHMEM0
  .bss.data_shmem0 (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_shmem0);
  } > SHMEM0

  .rpc_services : ALIGN(0x8)
  {
    KEEP(*libnm6407_io_nmc.a:*(.rpc_services));
  } > EMI0

  .rpc_services.bss (NOLOAD) : ALIGN(0x8)
  {
    *libnm6407_io_nmc.a:*(.rpc_services.bss);
  } > EMI0

  .nmgl_remote.bss (NOLOAD) : ALIGN(0x8)
  {
    *(.nmgl_remote.bss);
  } > EMI0
  .nmgl_remote : ALIGN(0x8)
  {
    *(.nmgl_remote);
  } > EMI0
  .data_DDR : ALIGN(0x8)
  {
    *(.data_DDR)
  } > EMI0






  .heap : ALIGN(0x8)
  {
    *(.heap);
	. = ORIGIN(C0_IMU0) + LENGTH(C0_IMU0);
    __HEAP_0_END = .;
  } > C0_IMU0
  .heap1 : ALIGN(0x8)
  {
    *(.heap1);
	. = ORIGIN(C0_IMU1) + LENGTH(C0_IMU1);
    __HEAP_1_END = .;
  } > C0_IMU1 
  .heap2 : ALIGN(0x8)
  {
    *(.heap2);
	. = ORIGIN(C0_IMU2) + LENGTH(C0_IMU2);
    __HEAP_2_END = .;
  } > C0_IMU2
  .heap3 : ALIGN(0x8)
  {
    *(.heap3);
	. = ORIGIN(C0_IMU3) + LENGTH(C0_IMU3);
    __HEAP_3_END = .;
  } > C0_IMU3  
  .heap4 : ALIGN(0x8)
  {
    *(.heap4);
	. = ORIGIN(C0_IMU4) + LENGTH(C0_IMU4);
    __HEAP_4_END = .;
  } > C0_IMU4
  .heap5 : ALIGN(0x8)
  {
    *(.heap5);
	. = ORIGIN(C0_IMU5) + LENGTH(C0_IMU5);
    __HEAP_5_END = .;
  } > C0_IMU5
  .heap6 : ALIGN(0x8)
  {
    *(.heap6);
	. = ORIGIN(C0_IMU6) + LENGTH(C0_IMU6);
    __HEAP_6_END = .;
  } > C0_IMU6
  .heap7 : ALIGN(0x8)
  {
    *(.heap7);
	. = ORIGIN(C0_IMU7) + LENGTH(C0_IMU7);
    __HEAP_7_END = .;
  } > C0_IMU7
  .heap8 : ALIGN(0x8)
  {
    *(.heap8);
	. = ORIGIN(SHMEM0) + LENGTH(SHMEM0);
    __HEAP_8_END = .;
  } > SHMEM0
  .heap10 : ALIGN(0x8)
  {
    *(.heap10);
	. = ORIGIN(EMI0) + LENGTH(EMI0);
    __HEAP_10_END = .;
  } > EMI0
  
}