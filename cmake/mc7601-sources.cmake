
file(GLOB mc7601_public_headers 
	include/hal/*.h
	include/*.h
	include/*.mlb)
file(GLOB mc7601_sources 
	${public_headers}
	src/1879vya1ya/*.asm
	src/1879vya1ya/*.c
	src/1879vya1ya/*.cpp	
	src/1879vya1ya/*.s
	src/1879vya1ya/*.S
	src/1879vya1ya/*.mlb
	src/1879vya1ya/*.h
	src/*.h
	src/*.cpp
	src/ringbuffer/*.cpp  
	src/ringbuffer/*.c
	src/ringbuffer/*.asm	
	src/ringbuffer/*.s
	src/ringbuffer/*.S
	src/ringbuffer/*.mlb
	src/ringbuffer/*.h
	src/nmc_all/*.cpp
	src/nmc_all/*.c
	src/nmc_all/*.asm	
	src/nmc_all/*.s
	src/nmc_all/*.S
	src/nmc_all/*.mlb
	src/nmc_all/*.h
	${CMAKE_CURRENT_LIST_FILE}
	)