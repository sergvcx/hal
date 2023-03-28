cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)


project(hal-host-tests
	LANGUAGES C CXX
	VERSION 0.1.0)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"  is_top_level)

if(is_top_level)
    enable_testing()
    find_package(hal REQUIRED)
endif()

set(board mc12101)
add_executable(${board}_open_close_test main.cpp)
target_link_libraries(${board}_open_close_test PUBLIC hal::hal_${board}_host)
add_test(NAME ${board}_open_close_test 
		COMMAND ${board}_open_close_test)