cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(hal-tests)

set(test_targets )

add_executable(mc12101-test mc12101.c)
list(APPEND test_targets mc12101-test)
add_executable(mc12101-remote-test mc12101_remote.c)
list(APPEND test_targets mc12101-remote-test)
add_executable(mb7707-test mb7707.c)
list(APPEND test_targets mb7707-test)
add_executable(mc12705-test mc12705.c)
list(APPEND test_targets mc12705-test)

target_link_directories(mc12101-test PRIVATE $ENV{MC12101}/lib)
target_include_directories(mc12101-test PRIVATE $ENV{MC12101}/include)
#target_link_libraries(mc12101-test PRIVATE mc12101load io_host$<$<CONFIG:Debug>:d>)
#target_link_libraries(mc12101-test PRIVATE mc12101load io_host>)

#target_link_directories(mc12705-test PRIVATE $ENV{MC12705}/lib)
#target_include_directories(mc12705-test PRIVATE $ENV{MC12705}/include)
#target_link_libraries(mc12705-test PRIVATE mc12705load io_host$<$<CONFIG:Debug>:d>)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/data/mc12101/dummy.abs
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/mc12101)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/data/mc12101/factorial.abs
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/mc12101)
#file(COPY ${CMAKE_CURRENT_LIST_DIR}/data/mc12101/sync.abs
#    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/mc12101)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/data/mc12705/factorial.abs
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/mc12705)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/data/mb7707/factorial.abs
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/mb7707)


foreach(target ${test_targets})
    target_link_libraries(${target} PRIVATE hal::hal)
    #target_compile_definitions(${target} PRIVATE _DEBUG )
    set_target_properties(${target} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
    target_compile_definitions(${target} 
        PRIVATE 
            $<$<CONFIG:Release>:_ITERATOR_DEBUG_LEVEL=0>
            $<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=2>
            )

    get_target_property(defs ${target} COMPILE_DEFINITIONS)
    list(FILTER defs EXCLUDE REGEX [[^NDEBUG$]])
    set_property(TARGET ${target} PROPERTY COMPILE_DEFINITIONS ${defs})
endforeach()



foreach(target ${test_targets})
    add_test(NAME ${target}
        COMMAND ${target}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/data)
endforeach()
