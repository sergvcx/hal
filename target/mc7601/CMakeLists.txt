cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)
set(CMAKE_TOOLCHAIN_FILE "${ROOT}/cmake/nmc-gcc-compile.cmake")
set(board mc7601)
set(CMAKE_CONFIGURATION_TYPES "Release;Debug;RelWithDebInfo;MinSizeRel")
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "build type")

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
option(HAL_MC7601_INSTALL "Generate target for installing hal_${board}" ${is_top_level})






set(ARCH nmc3)

include(${board}-sources.cmake)

target_sources(hal_${board} PRIVATE ${${board}_sources})
add_nmc_compiler_type_option("${${board}_sources}")

set_target_properties(hal_${board}
	PROPERTIES 
	LINKER_LANGUAGE CXX
	ARCHIVE_OUTPUT_DIRECTORY ${ROOT}/lib
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${ROOT}/lib
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${ROOT}/lib
	PUBLIC_HEADER "${${board}_public_headers}"
	SOVERSION ${PROJECT_VERSION_MAJOR}
	VERSION ${PROJECT_VERSION}
	)
target_include_directories(hal_${board} 
	PUBLIC 
		"$<BUILD_INTERFACE:${ROOT}/include>"
		"$<INSTALL_INTERFACE:include>"
	PRIVATE
		${ROOT}/src/target/1879vm6ya)
target_compile_options(hal_${board} PUBLIC $<$<COMPILE_LANGUAGE:ASM>:-m${ARCH};-Wa,-W-111;-Wa,-W-109;-Wa,-split_sir>)
target_compile_options(hal_${board} PUBLIC $<$<COMPILE_LANGUAGE:C,CXX>:-Wall;-m${ARCH}>)
target_compile_options(hal_${board} PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>)
target_compile_options(hal_${board} PUBLIC $<$<COMPILE_LANGUAGE:C>:-std=c99>)
set_source_files_properties(${ROOT}/src_printf_nmgcc/printf.cpp PROPERTY COMPILE_FLAGS -O0)

if(HAL_MC7601_INSTALL)
	install_board_rules(${board} ${ROOT}/cmake/hal_${board}-config.cmake.in)
endif()