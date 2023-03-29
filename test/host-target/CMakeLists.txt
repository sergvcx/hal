cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(hal-host-target-tests
	LANGUAGES C CXX
	VERSION 0.1.0)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"  is_top_level)
option(EXTERNAL_HAL "Build tests for external hal" OFF)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/test-service.cmake)

set(board mc12101)
	
if(is_top_level)
	enable_testing()
	find_package(hal COMPONENTS mc12101 mc12101_host REQUIRED)
endif()


set(board mc12101)
add_executable(open_close_test_${board} open_close/host/main.cpp)
add_nm_target(open_close_test_${board}_nm ${board} "Ninja" open_close/nm-part/build "open_close/target/main.cpp")
target_link_libraries(open_close_test_${board} PUBLIC hal::${board}_host)
