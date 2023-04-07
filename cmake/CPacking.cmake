include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(utils.cmake)

configure_common_config()

set(CPACK_PACKAGE_NAME hal)
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/..)
set(CPACK_NSIS_INSTALL_ROOT "C:\\\\Program Files\\\\Module")
set(CPACK_PACKAGE_VENDOR "RC Module")


set(boards "mc12101;mc7601;mc5103;mb7707")
foreach(board ${boards})
	string(TOUPPER ${board} BOARD)
	if(HAL_${BOARD})	
		list(APPEND CPACK_INSTALL_CMAKE_PROJECTS 
			"${CMAKE_CURRENT_LIST_DIR}/../build/${board}/target/release;hal-target;ALL;/"
			"${CMAKE_CURRENT_LIST_DIR}/../build/${board}/target/debug;hal-target;ALL;/"
			"${CMAKE_CURRENT_LIST_DIR}/../build/${board}/host;hal-host;ALL;/"
			)
	endif()
endforeach()
message("${CPACK_INSTALL_CMAKE_PROJECTS}")

# list(APPEND CPACK_INSTALL_CMAKE_PROJECTS 
# 	"${CMAKE_CURRENT_LIST_DIR}/../build/${board}/virtual/release;hal-virtaul;ALL;/"
# 	)

include(CPack)