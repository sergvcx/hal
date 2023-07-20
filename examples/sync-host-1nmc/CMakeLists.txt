cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(example-sync)

add_executable(example-sync-host pc-main.cpp)

target_link_directories(example-sync-host PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../lib)
target_link_libraries(example-sync-host PUBLIC hal::hal)
if(UNIX)
target_link_libraries(example-sync-host PUBLIC dl)
endif()
target_include_directories(example-sync-host PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../include)

add_custom_target(example-sync-nm ALL
    COMMAND make mc12101 OUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

