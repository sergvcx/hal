set(board mc5103)
set(host_name hal-${board}-host)
set(target_name hal-${board})
set(nm_generator Ninja)

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
	make/mc5103/Makefile
	${CMAKE_CURRENT_LIST_FILE})
file(GLOB target_headers include/*.h)

file(GLOB HOST_SOURCES 
	src/host/mc5103/*.*
	src/x86/*.*
	src/ringbuffer/*.*
	#src/host/host_io/*.*
	${CMAKE_CURRENT_LIST_FILE})
file(GLOB host_headers include/*.h)


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
set_target_properties(${host_name} PROPERTIES 
	PUBLIC_HEADER "${host_headers}"
	)  


execute_process(
	COMMAND ${CMAKE_COMMAND} 
	-B ${CMAKE_CURRENT_LIST_DIR}/make/mc5103/build 
	${CMAKE_CURRENT_LIST_DIR}/make/mc5103 
	#-DCMAKE_TOOLCHAIN_FILE=${CMAKE_CURRENT_LIST_DIR}/nmc-gcc-compile.cmake
	-G ${nm_generator}
	COMMENT "Building nm part")
if(WIN32)
	add_custom_target(${target_name}_build $ENV{NMC_GCC_TOOLPATH}/nmc4cmd.bat
		COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/make/mc5103/build )
else()
	add_custom_target(${target_name}_build COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/make/mc5103/build )
endif()
add_library(${target_name} INTERFACE ${target_sources} ${target_headers})
add_dependencies(${target_name} ${target_name}_build)

set_target_properties(${target_name} PROPERTIES 
	ADDITIONAL_CLEAN_FILES ${CMAKE_CURRENT_LIST_DIR}/lib/libhal-${board}.a #not working
	PUBLIC_HEADER "${target_headers}"
	)  

add_dependencies(${host_name} ${target_name})

install(FILES ${CMAKE_CURRENT_LIST_DIR}/lib/libhal_${board}.a
#		EXPORT hal
		DESTINATION lib)

install(TARGETS ${host_name}
#		EXPORT hal
		COMPONENT hal_${board}
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
		PUBLIC_HEADER DESTINATION include)
