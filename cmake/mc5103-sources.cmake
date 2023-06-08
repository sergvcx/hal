
file(GLOB mc5103_public_headers 
	${ROOT}/include/*.h
	${ROOT}/include/*.mlb)
file(GLOB mc5103_sources 
	${public_headers}
	src/1879vm5ya/*.asm
	src/1879vm5ya/*.c
	src/1879vm5ya/*.cpp	
	src/1879vm5ya/*.s
	src/1879vm5ya/*.S
	src/1879vm5ya/*.mlb
	src/1879vm5ya/*.h
	${ROOT}/src/*.h
	${ROOT}/src/*.cpp
	${ROOT}/src/ringbuffer/*.cpp  
	${ROOT}/src/ringbuffer/*.c
	${ROOT}/src/ringbuffer/*.asm	
	${ROOT}/src/ringbuffer/*.s
	${ROOT}/src/ringbuffer/*.S
	${ROOT}/src/ringbuffer/*.mlb
	${ROOT}/src/ringbuffer/*.h
	src/nmc_all/*.cpp
	src/nmc_all/*.c
	src/nmc_all/*.asm	
	src/nmc_all/*.s
	src/nmc_all/*.S
	src/nmc_all/*.mlb
	src/nmc_all/*.h
	${CMAKE_CURRENT_LIST_FILE}
	)