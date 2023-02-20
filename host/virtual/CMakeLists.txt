set(lib_name hal-virtual)
set(root ../..)

add_library(${lib_name} STATIC)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
    is_top_level)
option(HAL_VIRTUAL_INSTALL "Generate target for installing hal_virtual" ${is_top_level})









file(GLOB sources 
	${root}/src/virtual/*.cpp
	${root}/src/virtual/*.c
	${root}/src/virtual/*.h
	${root}/src/x86/*.*
	${root}/src/ringbuffer/*.*
	${root}/include/*.h
	${CMAKE_CURRENT_LIST_FILE})

set_target_properties(${lib_name} 
	PROPERTIES 
	LINKER_LANGUAGE CXX
	ARCHIVE_OUTPUT_DIRECTORY ${root}/lib
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${root}/lib
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${root}/lib)
target_include_directories(${lib_name} 
	PUBLIC 
		${root}/include 
	PRIVATE
		$ENV{NMPP}/include )

target_sources(${lib_name} PRIVATE ${sources})
#install host lib
if(HAL_VIRTUAL_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
	# configure_package_config_file(hal_virtual-config.cmake.in hal_virtual-config.cmake
 #    	INSTALL_DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	# write_basic_package_version_file(hal_virtual-config-version.cmake
 #    	COMPATIBILITY SameMajorVersion)
    install(TARGETS hal_virtual EXPORT hal_virtual_export
    	ARCHIVE 
    		COMPONENT hal_virtual-dev
    		DESTINATION lib
    	PUBLIC_HEADER 
    		COMPONENT hal_virtual-dev   
    		DESTINATION include)
    install(EXPORT hal_virtual_export
    	COMPONENT hal_virtual-dev
    	FILE hal_virtual-targets.cmake
    	DESTINATION lib/cmake/hal_virtual
    	NAMESPACE hal::)
	export(TARGETS hal_virtual
		FILE ${CMAKE_CURRENT_BINARY_DIR}/hal_virtual-targets.cmake
		NAMESPACE hal::)
	install(FILES
	    "${CMAKE_CURRENT_BINARY_DIR}/hal_virtual-config.cmake"
	    "${CMAKE_CURRENT_BINARY_DIR}/hal_virtual-config-version.cmake"
	    COMPONENT hal_virtual-dev
	    DESTINATION "lib/cmake/hal_virtual")
endif()

