cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(example-simple)

add_executable(example-simple-host host.cpp)

target_link_directories(example-simple-host PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../lib)
target_link_libraries(example-simple-host PUBLIC hal_x86)
if(UNIX)
target_link_libraries(example-simple-host PUBLIC dl)
endif()
target_include_directories(example-simple-host PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../include)

add_custom_target(example-simple-nm ALL
    COMMAND make mc12101 OUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

