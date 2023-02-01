cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)
set(CMAKE_TOOLCHAIN_FILE "${ROOT}/nmc-gcc-compile.cmake")
set(board mc5103)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

project(hal_${board} 
	LANGUAGES C CXX ASM 
	DESCRIPTION "Hal library for ${board}"
	VERSION 0.1.0)

add_library(hal_${board} STATIC)
add_library(hal::hal_${board} ALIAS hal_${board})

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
    is_top_level)

include(CMakePackageConfigHelpers)
include(${ROOT}/cmake/utils.cmake)
option(HAL_${board}_INSTALL "Generate target for installing hal_${board}" ${is_top_level})

set(ARCH nmc3)

file(GLOB public_headers 
	${ROOT}/include/*.h
	${ROOT}/include/*.mlb)
file(GLOB sources 
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
	)


target_sources(hal_${board} PRIVATE ${sources})

set_target_properties(hal_${board}
	PROPERTIES 
	LINKER_LANGUAGE CXX
	ARCHIVE_OUTPUT_DIRECTORY ${ROOT}/lib
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${ROOT}/lib
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${ROOT}/lib
	PUBLIC_HEADER "${public_headers}"
	SOVERSION ${PROJECT_VERSION_MAJOR}
	VERSION ${PROJECT_VERSION}
	)

add_nmc_compiler_type_option("${sources}")
target_include_directories(hal_${board} 
	PUBLIC 
		"$<BUILD_INTERFACE:${ROOT}/include>"
		"$<INSTALL_INTERFACE:include>" #install-prefix/include
	PRIVATE
		${ROOT}/src/target/1879vm6ya
	)
target_compile_options(hal_${board} PUBLIC $<$<COMPILE_LANGUAGE:ASM>:-m${ARCH};-Wa,-W-111;-Wa,-W-109;-Wa,-split_sir>)
target_compile_options(hal_${board} PUBLIC $<$<COMPILE_LANGUAGE:C,CXX>:-O2;-Wall;-m${ARCH}>)
target_compile_options(hal_${board} PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>)
target_compile_options(hal_${board} PUBLIC $<$<COMPILE_LANGUAGE:C>:-std=c99>)
target_compile_options(hal_${board} PUBLIC $<$<CONFIG:DEBUG>:-g>)
set_source_files_properties(${ROOT}/src_printf_nmgcc/printf.cpp PROPERTY COMPILE_FLAGS -O0)

install_board_rules(${board} ${ROOT}/cmake/hal_${board}-config.cmake.in)
