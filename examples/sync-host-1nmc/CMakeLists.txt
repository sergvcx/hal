cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(sync-host-1nmc)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"    is_top_level)

if(is_top_level)
	set(HAL_PATH "${CMAKE_CURRENT_LIST_DIR}/../.." CACHE PATH "Path to hal/lib/cmake/hal")
	list(APPEND CMAKE_PREFIX_PATH "${HAL_PATH}/lib/cmake/hal")
	find_package(hal COMPONENTS mc12101 REQUIRED)
endif()

add_subdirectory(host)


add_custom_target(nm-gen 
	COMMAND "C:\Program Files\Module\NMC-SDK\nmc4cmd.bat" .
	COMMAND ${CMAKE_COMMAND} -S ${CMAKE_CURRENT_LIST_DIR}/target -B ${CMAKE_CURRENT_LIST_DIR}/build_nm -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH} -G Ninja )

add_custom_target(nm-build ALL
	COMMAND "C:\Program Files\Module\NMC-SDK\nmc4cmd.bat" .
	COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/build_nm
	DEPENDS nm-gen)