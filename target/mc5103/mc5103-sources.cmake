
file(GLOB mc5103_public_headers 
	${ROOT}/include/*.h
	${ROOT}/include/*.mlb)
file(GLOB mc5103_sources 
	${public_headers}
	${ROOT}/src/target/1879vm5ya/*.asm
	${ROOT}/src/target/1879vm5ya/*.c
	${ROOT}/src/target/1879vm5ya/*.cpp	
	${ROOT}/src/target/1879vm5ya/*.s
	${ROOT}/src/target/1879vm5ya/*.S
	${ROOT}/src/target/1879vm5ya/*.mlb
	${ROOT}/src/target/1879vm5ya/*.h
	${ROOT}/src/ringbuffer/*.cpp  
	${ROOT}/src/ringbuffer/*.c
	${ROOT}/src/ringbuffer/*.asm	
	${ROOT}/src/ringbuffer/*.s
	${ROOT}/src/ringbuffer/*.S
	${ROOT}/src/ringbuffer/*.mlb
	${ROOT}/src/ringbuffer/*.h
	${ROOT}/src/target/nmc_all/*.cpp
	${ROOT}/src/target/nmc_all/*.c
	${ROOT}/src/target/nmc_all/*.asm	
	${ROOT}/src/target/nmc_all/*.s
	${ROOT}/src/target/nmc_all/*.S
	${ROOT}/src/target/nmc_all/*.mlb
	${ROOT}/src/target/nmc_all/*.h
	${CMAKE_CURRENT_LIST_FILE}
	)