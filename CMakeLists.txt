cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(hal 
	LANGUAGES C CXX
	VERSION 0.1.0)


option(MC12101_BUILD "Build hal for mc12101" ON)
option(MB7707_BUILD "Build hal for mb7707" OFF)
option(MC5103_BUILD "Build hal for mc5103" OFF)
option(MC7601_BUILD "Build hal for mc7601" OFF)
option(VIRTUAL_BUILD "Build hal x64" ON)
set(NM_GENERATOR "Unix Makefiles" CACHE STRING "NM Building generator")






include(${CMAKE_CURRENT_LIST_DIR}/cmake/utils.cmake)

if(MC12101_BUILD)
	set(HAL_MC12101_HOST_INSTALL ON)
	add_subdirectory(host/mc12101)
	add_nm_build_target(mc12101 ${NM_GENERATOR})
	message(STATUS "MC12101 include")
endif()

if(MB7707_BUILD)
	set(HAL_MB7707_HOST_INSTALL ON)
	add_subdirectory(host/mb7707)
	add_nm_build_target(mb7707 ${NM_GENERATOR})
	message(STATUS "MB7707 include")
endif()

if(MC5103_BUILD)
	set(HAL_MC5103_HOST_INSTALL ON)
	add_subdirectory(host/mc5103)
	add_nm_build_target(mc5103 ${NM_GENERATOR})
	message(STATUS "MC5103 include")
endif()

if(MC7601_BUILD)
	set(HAL_MC7601_HOST_INSTALL ON)
	add_subdirectory(host/mc7601)
	add_nm_build_target(mc7601 ${NM_GENERATOR})
	message(STATUS "MC7601 include")
endif()

if(VIRTUAL_BUILD)
	set(HAL_VIRTUAL_INSTALL ON)
	add_subdirectory(host/virtual)
	message(STATUS "VIRTUAL include")
endif()

#include(CPacking.cmake)