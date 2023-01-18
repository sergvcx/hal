cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)


set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

project(hal C CXX)

include(mc12101.cmake)
include(mb7707.cmake)
include(mc5103.cmake)
include(mc7601.cmake)
include(virtual.cmake)