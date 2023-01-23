set(host_name hal-mb7707)
set(target_name hal-mb7707-nm)
set(nm_generator Ninja)

file(GLOB target_sources 
	src/target/1879hb1ya/*.cpp
	src/target/1879hb1ya/*.c
	src/target/1879hb1ya/*.asm
	src/target/1879hb1ya/*.mlb
	src/target/1879hb1ya/*.S
	src/target/1879hb1ya/*.s
	src/target/nmc_all/*.cpp
	src/target/nmc_all/*.c
	src/target/nmc_all/*.asm
	src/target/nmc_all/*.mlb
	src/target/nmc_all/*.S
	src/target/nmc_all/*.s
	src/ringbuffer/*.*
	include/*.h
	make/mb7707/Makefile
	${CMAKE_CURRENT_LIST_FILE})

file(GLOB host_sources 
	src/host/mb7707/*.*
	src/x86/*.*
	src/ringbuffer/*.*
	include/*.h
	#src/io/host_io/*.*
	${CMAKE_CURRENT_LIST_FILE})


add_library(${host_name} STATIC ${host_sources})
set_target_properties(${host_name} 
	PROPERTIES 
	LINKER_LANGUAGE CXX
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_LIST_DIR}/lib
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_LIST_DIR}/lib)
target_include_directories(${host_name} PUBLIC 
	${CMAKE_CURRENT_LIST_DIR}/include 
	$ENV{MB7707}/libload 
	#${CMAKE_CURRENT_LIST_DIR}/src/io/host_io
	)
target_compile_definitions(${host_name} PUBLIC NM6405 $<$<CONFIG:Debug>:DEBUG> $<$<CONFIG:Release>:NDEBUG>)


execute_process(
	COMMAND ${CMAKE_COMMAND} 
	-B ${CMAKE_CURRENT_LIST_DIR}/make/mb7707/build 
	${CMAKE_CURRENT_LIST_DIR}/make/mb7707 
	#-DCMAKE_TOOLCHAIN_FILE=${CMAKE_CURRENT_LIST_DIR}/nmc-gcc-compile.cmake
	-G ${nm_generator}
	COMMENT "Building nm part")
if(WIN32)
	add_custom_target(${target_name} $ENV{NMC_GCC_TOOLPATH}/nmc4cmd.bat
		COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/make/mb7707/build )
else()
	add_custom_target(${target_name} COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/make/mb7707/build )
endif()

set_target_properties(${target_name} PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_CURRENT_LIST_DIR}/lib/libhal-mb7707.a)  #not working

add_dependencies(${host_name} ${target_name})