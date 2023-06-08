
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

#target_compile_options(${target_name} PUBLIC $<$<COMPILE_LANGUAGE:ASM>:-m${ARCH};-Wa,-W-111;-Wa,-W-109;-Wa,-split_sir>)
#target_compile_options(${target_name} PUBLIC $<$<COMPILE_LANGUAGE:C,CXX>:-Wall;-m${ARCH}>)
#target_compile_options(${target_name} PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>)
#target_compile_options(${target_name} PUBLIC $<$<COMPILE_LANGUAGE:C>:-std=c99>)