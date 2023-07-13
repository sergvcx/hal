cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

#unset($ENV{LANG})

project(hal 
	LANGUAGES C CXX
	DESCRIPTION "Hal library"
	)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

add_library(hal STATIC)
add_library(hal::hal ALIAS hal)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"   is_top_level)

option(HAL_MC12101 "Build hal for mc12101" OFF)
option(HAL_MB7707 "Build hal for mb7707" OFF)
option(HAL_MC5103 "Build hal for mc5103" OFF)
option(HAL_MC7601 "Build hal for mc7601" OFF)
option(HAL_X86    "Build hal for x86" OFF)
option(HAL_PACK "Create configuration for cpack and exit" OFF)
option(HAL_TESTS "Enable tests" OFF)
include(cmake/mb7707-sources.cmake)
include(cmake/mc5103-sources.cmake)
include(cmake/mc7601-sources.cmake)
include(cmake/mc12101-sources.cmake)
include(cmake/x86-sources.cmake)
include(cmake/utils.cmake)


target_compile_options(hal PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>)
target_compile_options(hal PUBLIC $<$<COMPILE_LANGUAGE:C>:-std=c99>)

target_include_directories(hal 
	PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/include
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/src
		${CMAKE_CURRENT_BINARY_DIR}
	)

add_custom_target(hal-version ALL
	COMMAND ${CMAKE_COMMAND}
	-D VERSION_FILE=${CMAKE_CURRENT_LIST_DIR}/version.txt
	-D BUILD_DIR=${CMAKE_CURRENT_BINARY_DIR}
	-D VERSION_NAME=HAL_VERSION
	-P "${CMAKE_CURRENT_LIST_DIR}/cmake/increment_build.cmake"
	DEPENDS version.txt "${CMAKE_CURRENT_LIST_DIR}/cmake/increment_build.cmake"
)

add_dependencies(hal hal-version)
if(HAL_X86)
	set(arch x86)
	target_sources(hal PRIVATE ${x86_sources})
	target_compile_definitions(hal PUBLIC $<$<CONFIG:Debug>:DEBUG> HAL_IO_HOST)
	# target_compile_definitions(hal PUBLIC $<$<CONFIG:Debug>:DEBUG>)
	target_include_directories(hal 
		PRIVATE 
			src/x86
			$ENV{MC12101}/include)
if(MSVC)
	#target_compile_definitions(hal PUBLIC _ITERATOR_DEBUG_LEVEL=0)
	target_compile_definitions(hal PUBLIC 
		$<$<CONFIG:Release>:_ITERATOR_DEBUG_LEVEL=0> 
        $<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=2> 
		$<$<CONFIG:Debug>:_DEBUG> 
    	_CRT_SECURE_NO_DEPRECATE
		)
endif()
	if(WIN32)
		#add_subdirectory(hal-run)
	else()
		target_link_libraries(hal PUBLIC dl)
	endif()
	if(MINGW)
		target_compile_options(hal PUBLIC -fpermissive)
	endif()
	enable_testing()
	add_subdirectory(test/x86)
	add_subdirectory(examples)
endif()
if(HAL_MC12101)
	set(arch mc12101)
	target_sources(hal PRIVATE ${mc12101_sources})	
	enable_language(ASM)
	add_nmc_compiler_type_option("${mc12101_sources}")
	target_compile_options(hal PUBLIC $<$<COMPILE_LANGUAGE:ASM>:-mnmc4;-Wa,-W-111;-Wa,-W-109;-Wa,-split_sir>)
	target_compile_options(hal PUBLIC $<$<COMPILE_LANGUAGE:C,CXX>:-Wall;-mnmc4;-fpermissive>)
	set_source_files_properties(src/printf_nmgcc/printf.cpp PROPERTY COMPILE_FLAGS -O0)
	target_include_directories(hal PRIVATE src/1879vm6ya $ENV{MC12101}/include)
	enable_testing()
	add_subdirectory(test/1879vm6ya)
endif()
if(HAL_MB7707)
	set(arch mb7707)
	target_sources(hal PRIVATE ${mb7707_sources})
	enable_language(ASM)
	add_nmc_compiler_type_option("${mb7707_sources}")
	target_compile_options(hal PUBLIC $<$<COMPILE_LANGUAGE:ASM>:-mnmc3;-Wa,-W-111;-Wa,-W-109;-Wa,-split_sir>)
	target_compile_options(hal PUBLIC $<$<COMPILE_LANGUAGE:C,CXX>:-Wall;-mnmc3>)
	set_source_files_properties(src/printf_nmgcc/printf.cpp PROPERTY COMPILE_FLAGS -O0)
	target_include_directories(hal PRIVATE src/1879vm6ya)
endif()
if(HAL_MC5103)
	set(arch mc5103)	
	target_sources(hal PRIVATE ${mc5103_sources})	
	enable_language(ASM)
	add_nmc_compiler_type_option("${mc5103_sources}")
	target_compile_options(hal PUBLIC $<$<COMPILE_LANGUAGE:ASM>:-mnmc3;-Wa,-W-111;-Wa,-W-109;-Wa,-split_sir>)
	target_compile_options(hal PUBLIC $<$<COMPILE_LANGUAGE:C,CXX>:-Wall;-mnmc3>)
	set_source_files_properties(src/printf_nmgcc/printf.cpp PROPERTY COMPILE_FLAGS -O0)
	target_include_directories(hal PRIVATE src/1879vm6ya)
endif()
if(HAL_MC7601)
	set(arch mc7601)
	target_sources(hal PRIVATE ${mc7601_sources})
	add_nmc_compiler_type_option(${mc7601_sources})
	enable_language(ASM)
	target_compile_options(hal PUBLIC $<$<COMPILE_LANGUAGE:ASM>:-mnmc3;-Wa,-W-111;-Wa,-W-109;-Wa,-split_sir>)
	target_compile_options(hal PUBLIC $<$<COMPILE_LANGUAGE:C,CXX>:-Wall;-mnmc3>)
	set_source_files_properties(src/printf_nmgcc/printf.cpp PROPERTY COMPILE_FLAGS -O0)
	target_include_directories(hal PRIVATE src/1879vm6ya)
endif()

set_target_properties(hal PROPERTIES
    DEBUG_POSTFIX d
	OUTPUT_NAME hal_${arch}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_LIST_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_LIST_DIR}/lib
)

#add_subdirectory(examples)


# if(HAL_PACK)
# 	include(cmake/CPacking.cmake)
# 	return()
# endif()


# include(${CMAKE_CURRENT_LIST_DIR}/cmake/utils.cmake)
# if(is_top_level)
# 	enable_testing()
# endif()

# if(HAL_TYPE STREQUAL "TARGET")
# 	add_subdirectory(target)
# elseif(HAL_TYPE STREQUAL "HOST")
# 	add_subdirectory(host)
# 	add_subdirectory(examples)
# 	if(HAL_TESTS)
# 		#add_subdirectory(test/host-target)
# 	endif()
# elseif(HAL_TYPE STREQUAL "VIRTUAL")
# 	add_subdirectory(virtual)
# 	if(HAL_TESTS)
# 		#add_subdirectory(test/virtual)
# 	endif()
# else()
# 	message(FATAL_ERROR "Wrong type of build (needed \"HOST\", \"TARGET\" or \"VIRTUAL\")")
# endif()

# if(HAL_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
# 	configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/hal_target-config.cmake.in 
# 		${config_dst}/hal_${board}-config.cmake
# 		INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hal_${board}
# 		PATH_VARS INCLUDE_INSTALL_DIR SYSCONFIG_INSTALL_DIR)
# 	write_basic_package_version_file(
# 		${config_dst}/hal_${board}-config-version.cmake
# 		VERSION "${PROJECT_VERSION}"
# 		COMPATIBILITY SameMajorVersion
# 		ARCH_INDEPENDENT)
# 	install(TARGETS ${target_name} EXPORT hal_${board}_export
# 		ARCHIVE 
# 			COMPONENT hal_${board}-dev
# 		PUBLIC_HEADER 
# 			COMPONENT hal_${board}-dev
# 			DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# 	install(EXPORT hal_${board}_export
# 		COMPONENT hal_${board}-dev
# 		FILE hal_${board}-targets.cmake
# 		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hal_${board}
# 		NAMESPACE hal::)
# 	export(TARGETS ${target_name}
# 		NAMESPACE hal::
# 		FILE ${config_dst}/hal_${board}-targets.cmake)
# 	install(FILES
# 		"${config_dst}/hal_${board}-config.cmake"
# 		"${config_dst}/hal_${board}-config-version.cmake"
# 		COMPONENT hal_${board}-dev
# 		DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/hal_${board}")
# endif()


