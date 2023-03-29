macro(add_nm_target target_name board nm_generator destination sources)

	execute_process(
		COMMAND ${CMAKE_COMMAND} 
		-B ${destination} 
		${CMAKE_CURRENT_LIST_DIR}/nm_part/${board}
        -D CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
		-G ${nm_generator}
		COMMENT "Building nm part for ${board}")

	#if(WIN32)
	#	add_custom_target(${target_name} ALL $ENV{NMC_GCC_TOOLPATH}/nmc4cmd.bat
	#		COMMAND ${CMAKE_COMMAND} --build ${destination_binary_dir} )
	#else()
	#	add_custom_target(${target_name} ALL COMMAND ${CMAKE_COMMAND} --build ${destination_binary_dir} )
	#endif()
	#add_custom_target(${target_name}_clean COMMAND ${CMAKE_COMMAND} --build ${destination_binary_dir} --target clean)
	#include(${destination_binary_dir}/hal_${board}-targets.cmake)
	#target_sources(${target_name} PRIVATE ${${board}_sources})

	#install(FILES 
    #	"$<TARGET_PROPERTY:hal::hal_${board},IMPORTED_LOCATION_NOCONFIG>"
    #	COMPONENT hal_${board}-dev
	#    DESTINATION lib)
	#install(FILES
	#	"${destination_binary_dir}/hal_${board}-targets.cmake"
	#    "${destination_binary_dir}/hal_${board}-config.cmake"
	#    "${destination_binary_dir}/hal_${board}-config-version.cmake"
	#    COMPONENT hal_${board}-dev
	#    DESTINATION "lib/cmake/hal_${board}")
endmacro()