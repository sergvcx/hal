cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(hal-tests)

set(test_targets )

add_executable(mc12101-halcp-test halcp.c)
list(APPEND test_targets mc12101-halcp-test)

foreach(target ${test_targets})
    target_link_libraries(${target} PRIVATE hal::hal)
    target_link_directories(${target} PRIVATE $ENV{MC12101}/lib)
    target_link_options(${target} PUBLIC
        -Wl,-T${CMAKE_CURRENT_LIST_DIR}/data/nmc1.ld
		-Wl,-T${CMAKE_CURRENT_LIST_DIR}/data/shared_mem.ld
    	-Wl,--whole-archive 
	      	-lmc12101load_nm 
			#$<$<CONFIG:DEBUG>:-lnm6407_io_nmc>
			-lnm6407_io_nmc
    	-Wl,--no-whole-archive
    )
endforeach()


foreach(target ${test_targets})
    add_test(NAME ${target}
        COMMAND ${target}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endforeach()