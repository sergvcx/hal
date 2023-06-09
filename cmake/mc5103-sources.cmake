
file(GLOB mc5103_public_headers 
	include/hal/*.h
	include/*.h
	include/*.mlb)
file(GLOB mc5103_sources 
	${public_headers}
	src/1879vm5ya/*.asm
	src/1879vm5ya/*.c
	src/1879vm5ya/*.cpp	
	src/1879vm5ya/*.s
	src/1879vm5ya/*.S
	src/1879vm5ya/*.mlb
	src/1879vm5ya/*.h
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