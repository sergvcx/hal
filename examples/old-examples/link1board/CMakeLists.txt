cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/../../cmake/nmc-gcc-compile.cmake)
project(link1board
	LANGUAGES C CXX ASM)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"    is_top_level)

if(is_top_level)
	set(HAL_PATH "${CMAKE_CURRENT_LIST_DIR}/../.." CACHE PATH "Path to hal")
	list(APPEND CMAKE_PREFIX_PATH "${HAL_PATH}/lib/cmake/hal")
	find_package(hal 0.1.0 COMPONENTS mc12101 REQUIRED)
endif()




add_executable(nmc0 src_target0/nm_main0.cpp)
add_executable(nmc1 src_target1/nm_main1.cpp)
target_link_libraries(nmc0 PUBLIC hal::mc12101)
target_link_libraries(nmc1 PUBLIC hal::mc12101)
target_link_directories(nmc0 PUBLIC $ENV{MC12101}/lib)
target_link_directories(nmc1 PUBLIC $ENV{MC12101}/lib)
target_link_options(nmc0 PUBLIC
	-Xlinker -Map=nmopengl_mc12101_fixed.map
	-Wl,-T${CMAKE_CURRENT_LIST_DIR}/../../app/templates/sometest/make_mc12101_2nmpu-gcc-ld/mc12101board-nmpu0.ld
   	-Wl,--whole-archive 
        -lmc12101load_nm 
   	-Wl,--no-whole-archive)
target_link_options(nmc1 PUBLIC
	-Xlinker -Map=nmopengl_mc12101_fixed.map
	-Wl,-T${CMAKE_CURRENT_LIST_DIR}/../../app/templates/sometest/make_mc12101_2nmpu-gcc-ld/mc12101board-nmpu1.ld
   	-Wl,--whole-archive 
        -lmc12101load_nm 
   	-Wl,--no-whole-archive)
