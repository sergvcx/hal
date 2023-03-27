cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(hal 
	LANGUAGES C CXX
	VERSION 0.1.0)


option(HAL_MC12101_BUILD "Build hal for mc12101" OFF)
option(HAL_MB7707_BUILD "Build hal for mb7707" OFF)
option(HAL_MC5103_BUILD "Build hal for mc5103" OFF)
option(HAL_MC7601_BUILD "Build hal for mc7601" OFF)
option(HAL_VIRTUAL_BUILD "Build hal x64" OFF)
set(NM_GENERATOR "Unix Makefiles" CACHE STRING "NM Building generator")



include(${CMAKE_CURRENT_LIST_DIR}/cmake/utils.cmake)

if(HAL_MC12101_BUILD)
	add_subdirectory(host/mc12101)
	add_nm_build_target(mc12101 ${NM_GENERATOR})
	message(STATUS "MC12101 include")
endif()

if(HAL_MB7707_BUILD)
	add_subdirectory(host/mb7707)
	add_nm_build_target(mb7707 ${NM_GENERATOR})
	message(STATUS "MB7707 include")
endif()

if(HAL_MC5103_BUILD)
	add_subdirectory(host/mc5103)
	add_nm_build_target(mc5103 ${NM_GENERATOR})
	message(STATUS "MC5103 include")
endif()

if(HAL_MC7601_BUILD)
	add_subdirectory(host/mc7601)
	add_nm_build_target(mc7601 ${NM_GENERATOR})
	message(STATUS "MC7601 include")
endif()

if(HAL_VIRTUAL_BUILD)
	add_subdirectory(host/virtual)
	message(STATUS "VIRTUAL include")
endif()

#include(CPacking.cmake)