cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)
project(hal-examples)

add_executable(simple simple/host.cpp)

set(target simple)
target_include_directories(${target}
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/../include)
target_link_libraries(${target} PUBLIC hal_x86d)
#target_link_libraries(${target} PUBLIC hal_mc12101d)
target_link_directories(${target} PUBLIC $ENV{HAL}/lib)