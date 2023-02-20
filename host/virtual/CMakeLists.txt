set(lib_name hal-virtual)
set(root ../..)

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
	ARCHIVE_OUTPUT_DIRECTORY ${root}/lib
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${root}/lib
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${root}/lib)
target_include_directories(${lib_name} 
	PUBLIC 
		${root}/include 
	PRIVATE
		$ENV{NMPP}/include )
target_compile_definitions(${lib_name} PUBLIC VIRTUAL_BOARD $<$<CONFIG:Debug>:DEBUG> $<$<CONFIG:Release>:NDEBUG>)
