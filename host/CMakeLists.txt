cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)
set(root ${CMAKE_CURRENT_LIST_DIR}/..)


project(hal-host
	LANGUAGES C CXX
	VERSION 0.1.2)

option(HAL_MC12101 "Build hal host part for mc12101"  OFF)
option(HAL_MB7707 "Build hal host part for mb7707" 	 OFF)
option(HAL_MC5103 "Build hal host part for mc5103" 	 OFF)
option(HAL_MC7601 "Build hal host part for mc7601" 	 OFF)

if(HAL_MC12101)
	add_library(mc12101_host STATIC)
	add_library(hal::mc12101_host ALIAS mc12101_host)
endif()
if(HAL_MB7707)
	add_library(mb7707_host STATIC)
	add_library(hal::mb7707_host ALIAS mb7707_host)
endif()
if(HAL_MC5103)
	add_library(mc5103_host STATIC)	
	add_library(hal::mc5103_host ALIAS mc5103_host)
endif()
if(HAL_MC7601)
	add_library(mc7601_host STATIC)
	add_library(hal::mc7601_host ALIAS mc7601_host)
endif()


string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
    is_top_level)

option(HAL_MC12101_INSTALL "Generate target for installing hal_mc12101_host" ${is_top_level})
option(HAL_MB7707_INSTALL "Generate target for installing hal_mb7707_host" ${is_top_level})
option(HAL_MC5103_INSTALL "Generate target for installing hal_mc5103_host" ${is_top_level})
option(HAL_MC7601_INSTALL "Generate target for installing hal_mc7601_host" ${is_top_level})
option(HAL_MC12101_TESTS "Generate target for installing hal_mc12101_host" OFF)
option(HAL_MB7707_TESTS "Generate target for installing hal_mb7707_host" OFF)
option(HAL_MC5103_TESTS "Generate target for installing hal_mc5103_host" OFF)
option(HAL_MC7601_TESTS "Generate target for installing hal_mc7601_host" OFF)
set(MC12101_BSP $ENV{MC12101} 			CACHE PATH "Path to mc12101 bsp")
set(MB7707_BSP $ENV{MB7707} 			CACHE PATH "Path to mb7707  bsp")
set(MC5103_BSP $ENV{MC5103}				CACHE PATH "Path to mc5103  bsp")
set(MC7601_BSP $ENV{MC7601}			   	CACHE PATH "Path to mc7601  bsp")
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)
include(GNUInstallDirs)
include(../cmake/utils.cmake)


set(ROOT ${CMAKE_CURRENT_LIST_DIR}/..)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)





set(MC12101_INCLUDE ${MC12101_BSP}/include)
set(MB7707_INCLUDE 	${MB7707_BSP}/libload)
set(MC5103_INCLUDE 	${MC5103_BSP}/libload)
set(MC7601_INCLUDE 	${MC7601_BSP}/include)


set(board_list "mc12101;mc5103;mb7707;mc7601")
#set(nm6403_board_list "mc7601;mc5103;mb7707")
#set(nm6405_board_list "mc12101")
if(is_top_level)
	enable_testing()
endif()

message(${CMAKE_INSTALL_PREFIX})

set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR})
set(SYSCONFIG_INSTALL_DIR ${CMAKE_INSTALL_SYSCONFDIR})

foreach(board ${board_list})

	set(target_name ${board}_host)
	if(NOT TARGET ${target_name})
		continue()
	endif()
	string(TOUPPER ${board} BOARD)
	file(GLOB public_headers ${ROOT}/include/*.h)
	file(GLOB sources 
		${public_headers}
		src/${board}/*.*
		src/*.*
		${ROOT}/src/x86/*.*
		${ROOT}/src/ringbuffer/*.*
		#src/host_io/*.*
		${CMAKE_CURRENT_LIST_FILE})


	target_sources(${target_name} PRIVATE ${sources})
	set_target_properties(${target_name} 
		PROPERTIES 
		CXX_STANDARD 11
		LINKER_LANGUAGE CXX
		ARCHIVE_OUTPUT_DIRECTORY ${ROOT}/lib/x86
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${ROOT}/lib/x86
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${ROOT}/lib/x86
		PUBLIC_HEADER "${public_headers}"
		DEBUG_POSTFIX d
		OUTPUT_NAME hal_${board}_host)	
	target_include_directories(${target_name} 
		PUBLIC 
			$<BUILD_INTERFACE:${ROOT}/include>
	  		$<INSTALL_INTERFACE:include> 
		PRIVATE
			${ROOT}/src/io/host_io
			${ROOT}/src
			${${BOARD}_INCLUDE}
		)
	target_compile_definitions(${target_name} PUBLIC NM6405 SILENT)
	if(UNIX)
		target_compile_options(${target_name} PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-fpermissive>)
	endif()
	
	#install host lib
	set(config_dst ${ROOT}/lib/cmake/hal_${board}_host)
	if(HAL_${BOARD}_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
		configure_common_config()
		configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/hal_host-config.cmake.in 
			${config_dst}/hal_${board}_host-config.cmake
	    	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hal_${board}_host
			PATH_VARS INCLUDE_INSTALL_DIR SYSCONFIG_INSTALL_DIR)
		write_basic_package_version_file(
			${config_dst}/hal_${board}_host-config-version.cmake
			VERSION "${PROJECT_VERSION}"
	    	COMPATIBILITY SameMajorVersion)
	    install(TARGETS ${target_name}  EXPORT hal_${board}_host_export
	    	ARCHIVE 	
	    		COMPONENT hal_${board}_host-dev
	    	PUBLIC_HEADER 
	    		COMPONENT hal_${board}_host-dev   )
	    install(EXPORT hal_${board}_host_export
	    	COMPONENT hal_${board}_host-dev
	    	FILE hal_${board}_host-targets.cmake
	    	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hal_${board}_host
	    	NAMESPACE hal::)
		export(TARGETS ${target_name}
			NAMESPACE hal::
			FILE ${config_dst}/hal_${board}_host-targets.cmake)
		install(FILES
		    "${config_dst}/hal_${board}_host-config.cmake"
		    "${config_dst}/hal_${board}_host-config-version.cmake"
		    COMPONENT hal_${board}_host-dev
		    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hal_${board}_host)
	endif()
	if(HAL_${BOARD}_TESTS)
		add_subdirectory(test)
	endif()
endforeach()




