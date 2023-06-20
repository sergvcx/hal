cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(hal-tests)

set(test_targets )

add_executable(mc12101-test mc12101.c)
list(APPEND test_targets mc12101-test)
add_executable(mb7707-test mb7707.c)
list(APPEND test_targets mb7707-test)

foreach(target ${test_targets})
    target_link_libraries(${target} PRIVATE hal::hal)
    set_target_properties(${target} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/mc12101/dummy.abs
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/mc12101)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/mc12101/factorial.abs
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/mc12101)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/mb7707/factorial.abs
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/mb7707)

foreach(target ${test_targets})
    add_test(NAME ${target}
        COMMAND ${target}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endforeach()
