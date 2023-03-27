
file(GLOB mb7707_public_headers 
	${ROOT}/include/*.h
	${ROOT}/include/*.mlb)
file(GLOB mb7707_sources 
	${public_headers}
	${ROOT}/src/target/1879hb1ya/*.asm
	${ROOT}/src/target/1879hb1ya/*.c
	${ROOT}/src/target/1879hb1ya/*.cpp	
	${ROOT}/src/target/1879hb1ya/*.s
	${ROOT}/src/target/1879hb1ya/*.S
	${ROOT}/src/target/1879hb1ya/*.mlb
	${ROOT}/src/target/1879hb1ya/*.h
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
	#${ROOT}/src/io/printf_nmcpp/*.c
	#${ROOT}/src/io/printf_nmcpp/*.h
	)