cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(sync-host-1nmc)

set(HAL_PATH "" CACHE PATH "Path to hal/lib/cmake/hal")
list(APPEND CMAKE_PREFIX_PATH "${HAL_PATH}/lib/cmake/hal")
message(${HAL_PATH})

add_subdirectory(host)


add_custom_target(nm-gen
	COMMAND ${CMAKE_COMMAND} -S ${CMAKE_CURRENT_LIST_DIR}/target -B ${CMAKE_CURRENT_LIST_DIR}/build_nm -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH} -G Ninja )

add_custom_target(nm-build ALL
	COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/build_nm
	DEPENDS nm-gen)