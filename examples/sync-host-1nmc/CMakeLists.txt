cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(sync-host-1nmc)

set(HAL_PATH "" CACHE PATH "Path to hal/lib/cmake/hal")
list(APPEND CMAKE_PREFIX_PATH "${HAL_PATH}/lib/cmake/hal")
message(${HAL_PATH})

add_subdirectory(host)


add_custom_target(nm-gen
	COMMAND ${CMAKE_COMMAND} -S target -B build_nm -G Ninja -D hal_DIR=${HAL_PATH}/lib/cmake/hal)

add_custom_target(nm-build ALL
	COMMAND ${CMAKE_COMMAND} --build build_nm
	DEPENDS nm-gen)