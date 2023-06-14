cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

project(libelf 
    VERSION 0.1.0)

add_library(libelf STATIC)


set(public_headers
    include/elf_set.h
    include/libelf.h
    include/libint.h)
set(sources 
    src/libelf.cpp
    src/liblib.cpp
    )

target_sources(libelf PRIVATE ${sources})
target_compile_definitions(libelf PUBLIC NM6405)
target_include_directories(libelf
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include)