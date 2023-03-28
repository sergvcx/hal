cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(HAL_TYPE "VIRTUAL" CACHE STRING "TARGET, HOST or VIRTUAL")
if(HAL_TYPE STREQUAL "TARGET")
	set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/nmc-gcc-compile.cmake)
endif()


project(hal VERSION 0.1.0)


option(HAL_MC12101 "Build hal for mc12101" OFF)
option(HAL_MB7707 "Build hal for mb7707" OFF)
option(HAL_MC5103 "Build hal for mc5103" OFF)
option(HAL_MC7601 "Build hal for mc7601" OFF)
set(HAL_MC12101_INSTALL ${HAL_MC12101})
set(HAL_MB7707_INSTALL ${HAL_MB7707})
set(HAL_MC5103_INSTALL ${HAL_MC5103})
set(HAL_MC7601_INSTALL ${HAL_MC7601})


string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"   is_top_level)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/utils.cmake)

if(HAL_TYPE STREQUAL "TARGET")
	add_subdirectory(target)
elseif(HAL_TYPE STREQUAL "HOST")
	add_subdirectory(host)
elseif(HAL_TYPE STREQUAL "VIRTUAL")
	add_subdirectory(virtual)
else()
	message(FATAL_ERROR "Wrong type of build (needed \"HOST\", \"TARGET\" or \"VIRTUAL\")")
endif()


if(is_top_level)
	include(cmake/CPacking.cmake)
endif()