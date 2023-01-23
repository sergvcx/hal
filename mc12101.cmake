set(host_name hal-mc12101)
set(target_name hal-mc12101-nm)
set(nm_generator Ninja)


#execute_process(COMMAND ${CMAKE_COMMAND} -E env Path="$ENV{NMC_GCC_TOOLPATH}/nmc4-ide/bin;$ENV{NMC_GCC_TOOLPATH}/nmc4-ide/lib;$ENV{Path}")
#execute_process(COMMAND ${CMAKE_COMMAND} -E environment)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB target_sources 
	src/target/1879vm6ya/*.cpp
	src/target/1879vm6ya/*.c
	src/target/1879vm6ya/*.asm
	src/target/1879vm6ya/*.mlb
	src/target/1879vm6ya/*.S
	src/target/1879vm6ya/*.s
	src/target/nmc_all/*.cpp
	src/target/nmc_all/*.c
	src/target/nmc_all/*.asm
	src/target/nmc_all/*.mlb
	src/target/nmc_all/*.S
	src/target/nmc_all/*.s
	src/ringbuffer/*.*
	include/*.h
	make/mc12101/Makefile
	${CMAKE_CURRENT_LIST_FILE})

file(GLOB host_sources 
	src/host/mc12101/*.*
	src/x86/*.*
	src/ringbuffer/*.*
	include/*.h
	#src/io/host_io/*.*
	${CMAKE_CURRENT_LIST_FILE})


add_library(${host_name} STATIC ${host_sources})
set_property(TARGET ${host_name} PROPERTY CXX_STANDARD 11)
set_target_properties(${host_name} 
	PROPERTIES 
	LINKER_LANGUAGE CXX
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_LIST_DIR}/lib
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_LIST_DIR}/lib)
target_include_directories(${host_name} PUBLIC 
	${CMAKE_CURRENT_LIST_DIR}/include 
	$ENV{MC12101}/include 
	${CMAKE_CURRENT_LIST_DIR}/src/io/host_io
	)
target_compile_definitions(${host_name} PUBLIC NM6405 $<$<CONFIG:Debug>:DEBUG> $<$<CONFIG:Release>:NDEBUG>)
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
if(WIN32)
	add_custom_target(${target_name} $ENV{NMC_GCC_TOOLPATH}/nmc4cmd.bat
		COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/make/mc12101/build )
else()
	add_custom_target(${target_name} COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/make/mc12101/build )
endif()

set_target_properties(${target_name} PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_CURRENT_LIST_DIR}/lib/libhal-mc12101.a)  #not working

add_dependencies(${host_name} ${target_name})

