cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(ROOT ${CMAKE_CURRENT_LIST_DIR}/..)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/nmc-gcc-compile.cmake)

project(hal-target
	LANGUAGES C CXX ASM 
	DESCRIPTION "Hal library, target part"
	VERSION 0.1.0)

option(HAL_MC12101 "Build hal for mc12101" OFF)
option(HAL_MB7707 "Build hal for mb7707" 	 OFF)
option(HAL_MC5103 "Build hal for mc5103" 	 OFF)
option(HAL_MC7601 "Build hal for mc7601" 	 OFF)

if(HAL_MC12101)
	add_library(mc12101 STATIC)
	add_library(hal_mc12101 ALIAS mc12101)
	add_library(hal::mc12101 ALIAS mc12101)
endif()
if(HAL_MB7707)
	add_library(mb7707 STATIC)
	add_library(hal_mb7707 ALIAS mb7707)
	add_library(hal::mb7707 ALIAS mb7707)
endif()
if(HAL_MC5103)
	add_library(mc5103 STATIC)	
	add_library(hal_mc5103 ALIAS mc5103)
	add_library(hal::mc5103 ALIAS mc5103)	
endif()
if(HAL_MC7601)
	add_library(mc7601 STATIC)
	add_library(hal_mc7601 ALIAS mc7601)
	add_library(hal::mc7601 ALIAS mc7601)
endif()



string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"   is_top_level)
option(HAL_MC12101_INSTALL "Build hal for mc12101" 	 ${is_top_level})
option(HAL_MB7707_INSTALL "Build hal for mb7707" 	 ${is_top_level})
option(HAL_MC5103_INSTALL "Build hal for mc5103" 	 ${is_top_level})
option(HAL_MC7601_INSTALL "Build hal for mc7601" 	 ${is_top_level})


include(CMakePackageConfigHelpers)
include(cmake/utils.cmake)
include(GNUInstallDirs)





set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR})
set(SYSCONFIG_INSTALL_DIR ${CMAKE_INSTALL_SYSCONFDIR})
set(nmc3_board_list "mc7601;mc5103;mb7707")
set(nmc4_board_list "mc12101")

set(board_list "mc12101;mc5103;mb7707;mc7601")
foreach(board ${board_list})
	set(target_name ${board})
	if(NOT TARGET ${target_name})
		continue()
	endif()

	include(cmake/${board}-sources.cmake)

	if(board IN_LIST nmc3_board_list)
		set(ARCH nmc3)
	elseif(board IN_LIST nmc4_board_list)
		set(ARCH nmc4)
	endif()
	if(NOT ARCH)
		message(FATAL_ERROR "ARCH NOT SET")
	endif()

	message(STATUS "Board: ${board}")
	message(STATUS "ARCH: ${ARCH}")


	target_sources(${target_name} PRIVATE ${${board}_sources})
	add_nmc_compiler_type_option("${${board}_sources}")

	set_target_properties(${target_name}
		PROPERTIES 
		LINKER_LANGUAGE CXX
		ARCHIVE_OUTPUT_DIRECTORY ${ROOT}/lib
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${ROOT}/lib
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${ROOT}/lib
		PUBLIC_HEADER "${${board}_public_headers}"
		SOVERSION ${PROJECT_VERSION_MAJOR}
		VERSION ${PROJECT_VERSION}
		DEBUG_POSTFIX d
		OUTPUT_NAME hal_${board}
		)
	target_include_directories(${target_name}
		PUBLIC 
			"$<BUILD_INTERFACE:${ROOT}/include>"
			"$<INSTALL_INTERFACE:include>" #install-prefix/include		
		)
	set(board_with_ext_include "mc12101;mb7707;mc5103;mc7601")
	if(board IN_LIST board_with_ext_include)
		target_include_directories(${target_name} PRIVATE src/1879vm6ya)
	endif()
	target_compile_options(${target_name} PUBLIC $<$<COMPILE_LANGUAGE:ASM>:-m${ARCH};-Wa,-W-111;-Wa,-W-109;-Wa,-split_sir>)
	target_compile_options(${target_name} PUBLIC $<$<COMPILE_LANGUAGE:C,CXX>:-Wall;-m${ARCH}>)
	target_compile_options(${target_name} PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>)
	target_compile_options(${target_name} PUBLIC $<$<COMPILE_LANGUAGE:C>:-std=c99>)
	set_source_files_properties(${ROOT}/src_printf_nmgcc/printf.cpp PROPERTY COMPILE_FLAGS -O0)

	string(TOUPPER ${board} BOARD)

	if(HAL_${BOARD}_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
		configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/hal_target-config.cmake.in hal_${board}-config.cmake
			INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hal_${board}
			PATH_VARS INCLUDE_INSTALL_DIR SYSCONFIG_INSTALL_DIR)
		write_basic_package_version_file(hal_${board}-config-version.cmake
			COMPATIBILITY SameMajorVersion)
		install(TARGETS ${target_name} EXPORT hal_${board}_export
			ARCHIVE 
				COMPONENT hal_${board}-dev
			PUBLIC_HEADER 
				COMPONENT hal_${board}-dev)
		install(EXPORT hal_${board}_export
			COMPONENT hal_${board}-dev
			FILE hal_${board}-targets.cmake
			DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hal_${board}
			NAMESPACE hal::)
		install(FILES
			"${CMAKE_CURRENT_BINARY_DIR}/hal_${board}-config.cmake"
			"${CMAKE_CURRENT_BINARY_DIR}/hal_${board}-config-version.cmake"
			COMPONENT hal_${board}-dev
			DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/hal_${board}")
	endif()
endforeach()
