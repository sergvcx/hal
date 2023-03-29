cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/../../../cmake/nmc-gcc-compile.cmake)
project(sync-host-nm
	LANGUAGES C CXX ASM)

find_package(hal COMPONENTS mc12101 REQUIRED)


add_executable(nm-part nm-main.cpp)
target_link_libraries(nm-part PUBLIC hal::mc12101)
target_link_directories(nm-part PUBLIC $ENV{MC12101}/lib)
target_link_options(nm-part PUBLIC
	-Wl,-T${CMAKE_CURRENT_LIST_DIR}/../../app/templates/sometest/make_mc12101_2nmpu-gcc-ld/mc12101board-nmpu1.ld
   	-Wl,--whole-archive 
        -lmc12101load_nm 
        -lnm6407_io_nmc
   	-Wl,--no-whole-archive)

