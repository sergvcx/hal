
#execute_process(COMMAND ${ROOT}/script.bat)
set(ENV{GCC_EXEC_PREFIX} /cygdrive/D/Module/NMC-SDK/nmc4-ide/lib/gcc/)
file(TO_NATIVE_PATH  $ENV{NMC_GCC_TOOLPATH}/nmc4-ide/bin path_bin)
file(TO_NATIVE_PATH  $ENV{NMC_GCC_TOOLPATH}/nmc4-ide/lib path_lib)
LIST(APPEND CMAKE_PROGRAM_PATH ${path_bin} ${path_lib})


macro(install_board_rules board config_template)
	string(TOUPPER ${board} BOARD)
	if(HAL_${board}_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
		configure_package_config_file(${config_template} hal_${board}-config.cmake
	    	INSTALL_DESTINATION "${CMAKE_CURRENT_LIST_DIR}")
		write_basic_package_version_file(hal_${board}-config-version.cmake
	    	COMPATIBILITY SameMajorVersion)
	    install(TARGETS hal_${board} EXPORT hal_${board}_export
	    	ARCHIVE 
	    		COMPONENT hal_${board}-dev
	    		DESTINATION lib
	    	PUBLIC_HEADER 
	    		COMPONENT hal_${board}-dev
		        DESTINATION include)
	    install(EXPORT hal_${board}_export
	    	COMPONENT hal_${board}-dev
	    	FILE hal_${board}-targets.cmake
	    	DESTINATION lib/cmake/hal_${board}
	    	NAMESPACE hal::)
		export(TARGETS hal_${board}
			FILE ${CMAKE_CURRENT_LIST_DIR}/hal_${board}-targets.cmake
			NAMESPACE hal::)
		install(FILES
		    "${CMAKE_CURRENT_BINARY_DIR}/hal_${board}-config.cmake"
		    "${CMAKE_CURRENT_BINARY_DIR}/hal_${board}-config-version.cmake"
		    COMPONENT hal_${board}-dev
		    DESTINATION "lib/cmake/hal_${board}")
	endif()
	unset(BOARD)
endmacro()

macro(add_nmc_compiler_type_option sources)
	foreach(file ${sources})
		get_filename_component(extension ${file} EXT)
		if(extension STREQUAL .asm)
			list(APPEND asm_files ${file})
		elseif(extension STREQUAL .S)
			list(APPEND s_files ${file})
		elseif(extension STREQUAL .s)
			list(APPEND s_files ${file})
		endif()
	endforeach()

	set_source_files_properties( ${asm_files} PROPERTY COMPILE_FLAGS "-mmas" )
	set_source_files_properties( ${s_files}   PROPERTY COMPILE_FLAGS "-mgas" )
endmacro()