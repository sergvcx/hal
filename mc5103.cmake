set(host_name hal-mc5103)
set(target_name hal-mc5103-nm)

file(GLOB TARGET_SOURCES 
	src/target/1879vm5ya/*.cpp
	src/target/1879vm5ya/*.c
	src/target/1879vm5ya/*.asm
	src/target/1879vm5ya/*.mlb
	src/target/1879vm5ya/*.S
	src/target/1879vm5ya/*.s
	src/target/nmc_all/*.cpp
	src/target/nmc_all/*.c
	src/target/nmc_all/*.asm
	src/target/nmc_all/*.mlb
	src/target/nmc_all/*.S
	src/target/nmc_all/*.s
	src/ringbuffer/*.*
	include/*.h
	make/mc5103/Makefile
	${CMAKE_CURRENT_LIST_FILE})

file(GLOB HOST_SOURCES 
	src/host/mc5103/*.*
	src/x86/*.*
	src/ringbuffer/*.*
	include/*.h
	src/io/host_io/*.*
	${CMAKE_CURRENT_LIST_FILE})


add_library(${host_name} STATIC ${HOST_SOURCES})
set_target_properties(${host_name} 
	PROPERTIES 
	LINKER_LANGUAGE CXX
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_LIST_DIR}/lib
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_LIST_DIR}/lib)
target_include_directories(${host_name} PUBLIC 
	${CMAKE_CURRENT_LIST_DIR}/include 
	$ENV{MC5103}/libload 
	${CMAKE_CURRENT_LIST_DIR}/src/io/host_io)
target_compile_definitions(${host_name} PUBLIC NM6405 $<$<CONFIG:Debug>:DEBUG> $<$<CONFIG:Release>:NDEBUG>)


add_custom_target(${target_name} make -C ${CMAKE_CURRENT_LIST_DIR}/make/mc5103 nmcgcc $<$<CONFIG:Debug>:DEBUG=y>
	SOURCES ${TARGET_SOURCES})

set_target_properties(${target_name} PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_CURRENT_LIST_DIR}/lib/libhal-mc5103.a)  #not working

add_dependencies(${host_name} ${target_name})