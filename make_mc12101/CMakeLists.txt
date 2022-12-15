cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(hal-mc12101)

set(CMAKE_CONFIGURATION_TYPES "Release;Debug")


file(GLOB NM_FILES 
	../src_1879vm6ya/*.cpp
	../src_1879vm6ya/*.c
	../src_1879vm6ya/*.asm
	../src_ringbuffer/*.*
	../include/*.h)

file(GLOB HOST_FILES 
		../src_mc12101_host/*.*
		../src_x86/*.*
		../src_ringbuffer/*.*
		../include/*.h
		../src_host_io/*.*)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../bin)

add_custom_target(hal-mc12101-nm ALL
	COMMAND make $<$<CONFIG:DEBUG>:DEBUG=y> ROOT=${CMAKE_CURRENT_LIST_DIR}/.. -f ${CMAKE_CURRENT_LIST_DIR}/Makefile nmcgcc 
	SOURCES ${NM_FILES})

add_custom_target(hal-mc12101-nm-legacy
	COMMAND make $<$<CONFIG:DEBUG>:DEBUG=y> ROOT=${CMAKE_CURRENT_LIST_DIR}/.. -f ${CMAKE_CURRENT_LIST_DIR}/Makefile legacy 
	SOURCES ${NM_FILES})
#target_include_directories(hal-mc12101-nm hal-mc12101-nm-legacy 
#	PUBLIC
#	../include
#	$ENV{MC12101}/include)


add_library(hal-mc12101-host STATIC
	${HOST_FILES}
)

set_target_properties( hal-mc12101-host
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../lib"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_LIST_DIR}/../lib"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_LIST_DIR}/../lib"
)

target_include_directories(hal-mc12101-host PUBLIC 
	../include 
	$ENV{MC12101}/include
	../src_host_io)
target_compile_definitions(hal-mc12101-host PUBLIC 
	$<$<CONFIG:DEBUG>:DEBUG> 
	NM6405)
