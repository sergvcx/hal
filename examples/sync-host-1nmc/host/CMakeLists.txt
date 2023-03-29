cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(sync-host-pc
	LANGUAGES C CXX)

find_package(hal COMPONENTS mc12101_host REQUIRED)


add_executable(host pc-main.cpp)
target_link_libraries(host PUBLIC hal::mc12101_host mc12101load)
target_link_directories(host PUBLIC $ENV{MC12101}/lib)

