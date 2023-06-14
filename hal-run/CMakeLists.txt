cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

project(hal-run 
    VERSION 0.1.0)

#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
add_executable(hal-run)


set(public_headers
    include/nmrun.h)
set(sources 
    src/main.cpp
    src/elf_analysis.cpp
    src/dump.cpp
    src/command_line.cpp)


add_subdirectory(libelf)
target_sources(hal-run PRIVATE ${sources})
target_include_directories(hal-run
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../hal/include    
        libelf/include
        $ENV{MC12101}/include)
target_link_directories(hal-run PUBLIC $ENV{MC12101}/lib)
target_compile_definitions(hal-run PUBLIC _ITERATOR_DEBUG_LEVEL=0 NM_RPC)
#target_compile_definitions(hal-run PUBLIC _ITERATOR_DEBUG_LEVEL=2 NM_RPC)
#target_compile_definitions(hal-run PUBLIC _ITERATOR_DEBUG_LEVEL=0)
target_link_libraries(hal-run PUBLIC hal libelf mc12101load io_host)

set_target_properties(hal-run
    PROPERTIES 
    DEBUG_POSTFIX d
    VERSION ${PROJECT_VERSION}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../bin
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_LIST_DIR}/../bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_LIST_DIR}/../bin
)
