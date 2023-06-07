cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(HAL_TYPE "VIRTUAL" CACHE STRING "TARGET, HOST or VIRTUAL")
if(HAL_TYPE STREQUAL "TARGET")
	set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/nmc-gcc-compile.cmake)
endif()


project(hal VERSION 0.1.0)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"   is_top_level)

option(HAL_MC12101 "Build hal for mc12101" OFF)
option(HAL_MB7707 "Build hal for mb7707" OFF)
option(HAL_MC5103 "Build hal for mc5103" OFF)
option(HAL_MC7601 "Build hal for mc7601" OFF)
option(HAL_PACK "Create configuration for cpack and exit" OFF)
option(HAL_TESTS "Enable tests" ON)
#set(boards "mc12101;mc7601;mc5103;mb7707")



if(HAL_PACK)
	include(cmake/CPacking.cmake)
	return()
endif()


include(${CMAKE_CURRENT_LIST_DIR}/cmake/utils.cmake)
if(is_top_level)
	enable_testing()
endif()

if(HAL_TYPE STREQUAL "TARGET")
	add_subdirectory(target)
elseif(HAL_TYPE STREQUAL "HOST")
	add_subdirectory(host)
	if(HAL_TESTS)
		#add_subdirectory(test/host-target)
	endif()
elseif(HAL_TYPE STREQUAL "VIRTUAL")
	add_subdirectory(virtual)
	if(HAL_TESTS)
		#add_subdirectory(test/virtual)
	endif()
else()
	message(FATAL_ERROR "Wrong type of build (needed \"HOST\", \"TARGET\" or \"VIRTUAL\")")
endif()


