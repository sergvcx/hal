set(lib_name hal-virtual)

file(GLOB sources 
	src/virtual/*.cpp
	src/virtual/*.c
	src/virtual/*.h
	src/x86/*.*
	src/ringbuffer/*.*
	include/*.h
	${CMAKE_CURRENT_LIST_FILE})


add_library(${lib_name} STATIC ${sources})
set_target_properties(${lib_name} 
	PROPERTIES 
	LINKER_LANGUAGE CXX
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_LIST_DIR}/lib
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_LIST_DIR}/lib)
target_include_directories(${lib_name} PUBLIC 
	${CMAKE_CURRENT_LIST_DIR}/include 
	$ENV{NMPP}/include )
target_compile_definitions(${lib_name} PUBLIC VIRTUAL_BOARD $<$<CONFIG:Debug>:DEBUG> $<$<CONFIG:Release>:NDEBUG>)
