cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)


set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

if(NOT PLATFORMS)
	set(PLATFORMS mc12101 mb7707 mc5103 mc7601 virtual)
endif()

project(hal C CXX)

if(mc12101 IN_LIST PLATFORMS)
	include(${CMAKE_CURRENT_LIST_DIR}/mc12101.cmake)
	message(STATUS "MC12101 include")
endif()

if(mb7707 IN_LIST PLATFORMS)
	include(${CMAKE_CURRENT_LIST_DIR}/mb7707.cmake)
	message(STATUS "MB7707 include")
endif()

if(mc5103 IN_LIST PLATFORMS)
	include(${CMAKE_CURRENT_LIST_DIR}/mc5103.cmake)
	message(STATUS "MC5103 include")
endif()

if(mc7601 IN_LIST PLATFORMS)
	include(${CMAKE_CURRENT_LIST_DIR}/mc7601.cmake)
	message(STATUS "MC7601 include")
endif()

if(virtual IN_LIST PLATFORMS)
	include(${CMAKE_CURRENT_LIST_DIR}/virtual.cmake)
	message(STATUS "VIRTUAL include")
endif()