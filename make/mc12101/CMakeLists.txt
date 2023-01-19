cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)
set(CMAKE_TOOLCHAIN_FILE "${ROOT}/nmc-gcc-compile.cmake")
#execute_process(COMMAND ${ROOT}/script.bat)
set(ENV{GCC_EXEC_PREFIX} /cygdrive/D/Module/NMC-SDK/nmc4-ide/lib/gcc/)
file(TO_NATIVE_PATH  $ENV{NMC_GCC_TOOLPATH}/nmc4-ide/bin path_bin)
file(TO_NATIVE_PATH  $ENV{NMC_GCC_TOOLPATH}/nmc4-ide/lib path_lib)
LIST(APPEND CMAKE_PROGRAM_PATH ${path_bin} ${path_lib})

project(hal-mc12101-nm C CXX ASM)

set(ARCH nmc4)


file(GLOB SOURCES 
	${ROOT}/src/target/1879vm6ya/*.asm
	${ROOT}/src/target/1879vm6ya/*.c
	${ROOT}/src/target/1879vm6ya/*.cpp	
	${ROOT}/src/target/1879vm6ya/*.s
	${ROOT}/src/target/1879vm6ya/*.S
	${ROOT}/src/target/1879vm6ya/*.mlb
	${ROOT}/src/target/1879vm6ya/*.h
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
add_library(hal_mc12101_nm STATIC ${SOURCES})

foreach(file ${SOURCES})
	#message(STATUS ${file})
	get_filename_component(extension ${file} EXT)
	#message(STATUS ${extension})
	if(extension STREQUAL .asm)
		list(APPEND asm_files ${file})
		#message(STATUS asm_file)
	elseif(extension STREQUAL .S)
		list(APPEND s_files ${file})
		#message(STATUS S_file)
	elseif(extension STREQUAL .s)
		list(APPEND s_files ${file})
		#message(STATUS s_file)
	endif()
endforeach()
#message(STATUS "${asm_files}")
#message(STATUS "${s_files}")


set_target_properties(hal_mc12101_nm
	PROPERTIES 
	LINKER_LANGUAGE CXX
	ARCHIVE_OUTPUT_DIRECTORY ${ROOT}/lib
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${ROOT}/lib
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${ROOT}/lib
	)

set_source_files_properties( ${asm_files}
		PROPERTY
		COMPILE_FLAGS "-mmas"
		)
set_source_files_properties( ${s_files}
		PROPERTY
		COMPILE_FLAGS "-mgas"
		)

target_compile_options(hal_mc12101_nm PUBLIC $<$<COMPILE_LANGUAGE:ASM>:-m${ARCH};-Wa,-W-111;-Wa,-W-109;-Wa,-split_sir>)
target_compile_options(hal_mc12101_nm PUBLIC $<$<COMPILE_LANGUAGE:C,CXX>:-O2;-Wall;-m${ARCH}>)
target_compile_options(hal_mc12101_nm PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>)
target_compile_options(hal_mc12101_nm PUBLIC $<$<COMPILE_LANGUAGE:C>:-std=c99>)
target_compile_options(hal_mc12101_nm PUBLIC $<$<CONFIG:DEBUG>:-g>)
set_source_files_properties(${ROOT}/src_printf_nmgcc/printf.cpp PROPERTY COMPILE_FLAGS -O0)
target_include_directories(hal_mc12101_nm PUBLIC ${ROOT}/include ${ROOT}/src/target/1879vm6ya)


# add_custom_command(TARGET hal_mc12101_nm PRE_BUILD
# 		COMMAND $ENV{NMC_GCC_TOOLPATH}/nmc4cmd.bat
# 		)
