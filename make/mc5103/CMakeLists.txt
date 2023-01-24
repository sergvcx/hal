cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)
set(CMAKE_TOOLCHAIN_FILE "${ROOT}/nmc-gcc-compile.cmake")
#execute_process(COMMAND ${ROOT}/script.bat)
set(ENV{GCC_EXEC_PREFIX} /cygdrive/D/Module/NMC-SDK/nmc4-ide/lib/gcc/)
file(TO_NATIVE_PATH  $ENV{NMC_GCC_TOOLPATH}/nmc4-ide/bin path_bin)
file(TO_NATIVE_PATH  $ENV{NMC_GCC_TOOLPATH}/nmc4-ide/lib path_lib)
LIST(APPEND CMAKE_PROGRAM_PATH ${path_bin} ${path_lib})

set(PROJECT hal_mc5103)
project(${PROJECT} C CXX ASM)

set(ARCH nmc3)


file(GLOB SOURCES 
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
	)
add_library(${PROJECT} STATIC ${SOURCES})

foreach(file ${SOURCES})
	get_filename_component(extension ${file} EXT)
	if(extension STREQUAL .asm)
		list(APPEND asm_files ${file})
	elseif(extension STREQUAL .S)
		list(APPEND s_files ${file})
	elseif(extension STREQUAL .s)
		list(APPEND s_files ${file})
	endif()
endforeach()


set_target_properties(${PROJECT}
	PROPERTIES 
	LINKER_LANGUAGE CXX
	ARCHIVE_OUTPUT_DIRECTORY ${ROOT}/lib
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${ROOT}/lib
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${ROOT}/lib
	)

set_source_files_properties( ${asm_files} PROPERTY COMPILE_FLAGS "-mmas" )
set_source_files_properties( ${s_files}   PROPERTY COMPILE_FLAGS "-mgas" )

target_compile_options(${PROJECT} PUBLIC $<$<COMPILE_LANGUAGE:ASM>:-m${ARCH};-Wa,-W-111;-Wa,-W-109;-Wa,-split_sir>)
target_compile_options(${PROJECT} PUBLIC $<$<COMPILE_LANGUAGE:C,CXX>:-O2;-Wall;-m${ARCH}>)
target_compile_options(${PROJECT} PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>)
target_compile_options(${PROJECT} PUBLIC $<$<COMPILE_LANGUAGE:C>:-std=c99>)
target_compile_options(${PROJECT} PUBLIC $<$<CONFIG:DEBUG>:-g>)
set_source_files_properties(${ROOT}/src_printf_nmgcc/printf.cpp PROPERTY COMPILE_FLAGS -O0)
target_include_directories(${PROJECT} PUBLIC ${ROOT}/include ${ROOT}/src/target/1879vm6ya)
