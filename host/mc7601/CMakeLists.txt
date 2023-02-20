cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)
set(root ${CMAKE_CURRENT_LIST_DIR}/..)
set(board mc7601)

project(hal_${board}_host
	LANGUAGES C CXX
	VERSION 0.1.0)

add_library(hal_${board}_host STATIC)
add_library(hal::hal_${board}_host ALIAS hal_${board}_host)


string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
    is_top_level)

include(CMakePackageConfigHelpers)
option(HAL_MC7601_HOST_INSTALL "Generate target for installing hal_${board}_host" ${is_top_level})







set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB public_headers include/*.h)
file(GLOB sources 
	${public_headers}
	src/host/mc7601/*.*
	src/x86/*.*
	src/ringbuffer/*.*
	#src/host/host_io/*.*
	${CMAKE_CURRENT_LIST_FILE})


target_sources(hal_${board}_host PRIVATE ${sources})
set_target_properties(hal_${board}_host 
	PROPERTIES 
	LINKER_LANGUAGE CXX
	ARCHIVE_OUTPUT_DIRECTORY ${root}/lib
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${root}/lib
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${root}/lib
	PUBLIC_HEADER "${public_headers}")
target_include_directories(hal_${board}_host 
	PUBLIC 
		$<BUILD_INTERFACE:${root}/include>
  		$<INSTALL_INTERFACE:include> 
	PRIVATE
		$ENV{MC7601}/libload 
		${root}/src/io/host_io)
target_compile_definitions(hal_${board}_host PUBLIC NM6405)
if(UNIX)
	target_compile_options(hal_${board}_host PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-fpermissive>)
endif()



#install host lib
if(HAL_MC7601_HOST_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
	configure_package_config_file(hal_${board}_host-config.cmake.in hal_${board}_host-config.cmake
    	INSTALL_DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	write_basic_package_version_file(hal_${board}_host-config-version.cmake
    	COMPATIBILITY SameMajorVersion)
    install(TARGETS hal_${board}_host EXPORT hal_${board}_host_export
    	ARCHIVE 
    		COMPONENT hal_${board}_host-dev
    		DESTINATION lib
    	PUBLIC_HEADER 
    		COMPONENT hal_${board}_host-dev   
    		DESTINATION include)
    install(EXPORT hal_${board}_host_export
    	COMPONENT hal_${board}_host-dev
    	FILE hal_${board}_host-targets.cmake
    	DESTINATION lib/cmake/hal_${board}_host
    	NAMESPACE hal::)
	export(TARGETS hal_${board}_host
		FILE ${CMAKE_CURRENT_BINARY_DIR}/hal_${board}_host-targets.cmake
		NAMESPACE hal::)
	install(FILES
	    "${CMAKE_CURRENT_BINARY_DIR}/hal_${board}_host-config.cmake"
	    "${CMAKE_CURRENT_BINARY_DIR}/hal_${board}_host-config-version.cmake"
	    COMPONENT hal_${board}_host-dev
	    DESTINATION "lib/cmake/hal_${board}_host")
endif()

