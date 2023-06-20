
file(GLOB mc12101_public_headers 
	${ROOT}/include/*.h
	${ROOT}/include/*.mlb)
file(GLOB mc12101_sources 
	${public_headers}
	src/1879vm6ya/*.asm
	src/1879vm6ya/*.c
	src/1879vm6ya/*.cpp	
	src/1879vm6ya/*.s
	src/1879vm6ya/*.S
	src/1879vm6ya/*.mlb
	src/1879vm6ya/*.h
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