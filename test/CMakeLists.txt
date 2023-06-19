cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(hal-tests)

set(target test_mc12101)
add_executable(${target} mc12101.c)
target_link_libraries(${target} PRIVATE hal::hal)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/mc12101/dummy.abs
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/mc12101)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/mc12101/factorial.abs
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/mc12101)

add_test(NAME test_mc12101
    COMMAND test_mc12101)