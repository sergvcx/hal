cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(hal-tests)

set(app_targets )

#add_executable(mc12101-halcp-test halcp.c)
#list(APPEND test_targets mc12101-halcp-test)
add_executable(mc12101-target-test0 target.c)
add_executable(mc12101-target-test1 target.c)

list(APPEND app_targets mc12101-target-test0 mc12101-target-test1)

target_link_options(mc12101-target-test0 PUBLIC -Wl,-T${CMAKE_CURRENT_LIST_DIR}/data/nmc0.ld)
target_link_options(mc12101-target-test1 PUBLIC -Wl,-T${CMAKE_CURRENT_LIST_DIR}/data/nmc1.ld)
target_compile_definitions(mc12101-target-test0 PUBLIC CORE=0)
target_compile_definitions(mc12101-target-test1 PUBLIC CORE=1)
set_target_properties(mc12101-target-test0 PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/data/nmc0.ld)
set_target_properties(mc12101-target-test1 PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/data/nmc1.ld)


foreach(target ${app_targets})
    target_link_libraries(${target} PRIVATE hal::hal)
    target_link_directories(${target} PRIVATE $ENV{MC12101}/lib)
    target_link_options(${target} PUBLIC
		-Wl,-T${CMAKE_CURRENT_LIST_DIR}/data/shared_mem.ld
    	-Wl,--whole-archive 
	      	-lmc12101load_nm 
			#$<$<CONFIG:DEBUG>:-lnm6407_io_nmc>
			-lnm6407_io_nmc
    	-Wl,--no-whole-archive
    )
endforeach()

add_test(NAME test-mc12101-target
    COMMAND python ${CMAKE_CURRENT_LIST_DIR}/run_parallel.py mc12101-target-test0.abs mc12101-target-test1.abs
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

