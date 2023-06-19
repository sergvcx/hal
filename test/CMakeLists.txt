cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(hal-tests)

set(test_targets )

add_executable(test_mc12101 mc12101.c)
list(APPEND test_targets test_mc12101)
add_executable(test_mb7707 mb7707.c)
list(APPEND test_targets test_mb7707)

foreach(target ${test_targets})
    target_link_libraries(${target} PRIVATE hal::hal)
endforeach()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/mc12101/dummy.abs
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/mc12101)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/mc12101/factorial.abs
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/mc12101)


add_test(NAME test_mc12101
    COMMAND test_mc12101)