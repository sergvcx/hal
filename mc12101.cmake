set(board mc12101)
set(host_name hal_${board}_host)
set(target_name hal_${board})
set(nm_generator Ninja)


#execute_process(COMMAND ${CMAKE_COMMAND} -E env Path="$ENV{NMC_GCC_TOOLPATH}/nmc4-ide/bin;$ENV{NMC_GCC_TOOLPATH}/nmc4-ide/lib;$ENV{Path}")
#execute_process(COMMAND ${CMAKE_COMMAND} -E environment)
add_library(${host_name} STATIC)


string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
    is_top_level)

include(CMakePackageConfigHelpers)
option(HAL_${board}_HOST_INSTALL "Generate target for installing hal_${board}_host" ${is_top_level})
option(HAL_${board}_INSTALL "Generate target for installing hal_${board}" ${is_top_level})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB host_public_headers include/*.h)
file(GLOB host_sources 
	${host_public_headers}
	src/host/mc12101/*.*
	src/x86/*.*
	src/ringbuffer/*.*
	#src/host/host_io/*.*
	${CMAKE_CURRENT_LIST_FILE})


target_sources(${host_name} PRIVATE ${host_sources})
set_property(TARGET ${host_name} PROPERTY CXX_STANDARD 11)
set_target_properties(${host_name} 
	PROPERTIES 
	LINKER_LANGUAGE CXX
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_LIST_DIR}/lib
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_LIST_DIR}/lib
	PUBLIC_HEADER "${host_public_headers}")
target_include_directories(${host_name} PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  	$<INSTALL_INTERFACE:include> 
  	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/io/host_io>
	$ENV{MC12101}/include 	
	)
target_compile_definitions(${host_name} PUBLIC NM6405 $<$<CONFIG:Debug>:DEBUG> $<$<CONFIG:Release>:NDEBUG> SILENT)
if(UNIX)
	target_compile_options(${host_name} PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-fpermissive>)
endif()


execute_process(
	COMMAND ${CMAKE_COMMAND} 
	-B ${CMAKE_CURRENT_LIST_DIR}/make/mc12101/build 
	${CMAKE_CURRENT_LIST_DIR}/make/mc12101 
	#-DCMAKE_TOOLCHAIN_FILE=${CMAKE_CURRENT_LIST_DIR}/nmc-gcc-compile.cmake
	-G ${nm_generator}
	COMMENT "Building nm part")
include(make/mc12101/hal_mc12101-targets.cmake)
if(WIN32)
	add_custom_target(${target_name}_build ALL $ENV{NMC_GCC_TOOLPATH}/nmc4cmd.bat
		COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/make/mc12101/build )
else()
	add_custom_target(${target_name}_build ALL COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/make/mc12101/build )
endif()
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR}/make/mc12101/build)
#find_package(hal_mc12101 REQUIRED)

#install host lib
if(HAL_${board}_HOST_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
	configure_package_config_file(cmake/${host_name}-config.cmake.in ${host_name}-config.cmake
    	INSTALL_DESTINATION "${CMAKE_CURRENT_LIST_DIR}")
	write_basic_package_version_file(${host_name}-config-version.cmake
    	COMPATIBILITY SameMajorVersion)
    install(TARGETS ${host_name} EXPORT ${host_name}_export
    	ARCHIVE 
    		COMPONENT ${host_name}-dev
    		DESTINATION lib
    	PUBLIC_HEADER 
    		COMPONENT ${host_name}-dev   
    		DESTINATION include)
    install(EXPORT ${host_name}_export
    	COMPONENT ${host_name}-dev
    	FILE ${host_name}-targets.cmake
    	DESTINATION lib/cmake/${host_name}
    	NAMESPACE hal::)
	export(TARGETS ${host_name}
		FILE ${CMAKE_CURRENT_LIST_DIR}/${host_name}-targets.cmake
		NAMESPACE hal::)
	install(FILES
	    "${CMAKE_CURRENT_BINARY_DIR}/${host_name}-config.cmake"
	    "${CMAKE_CURRENT_BINARY_DIR}/${host_name}-config-version.cmake"
	    COMPONENT ${host_name}-dev
	    DESTINATION "lib/cmake/${host_name}")
endif()

#install target lib
if(HAL_${board}_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    install(FILES 
    	"$<TARGET_PROPERTY:hal::hal_${board},IMPORTED_LOCATION_NOCONFIG>"
    	COMPONENT hal_${board}-dev
	    DESTINATION lib)
	install(FILES
		"make/mc12101/hal_${board}-targets.cmake"
	    "make/mc12101/build/hal_${board}-config.cmake"
	    "make/mc12101/build/hal_${board}-config-version.cmake"
	    COMPONENT hal_${board}-dev
	    DESTINATION "lib/cmake/hal_${board}")
endif()
