set(CPACK_PACKAGE_NAME hal)
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(CPACK_NSIS_INSTALL_ROOT "C:\\\\Program Files\\\\Module")
# SET(CPACK_INSTALL_CMAKE_PROJECTS  
# 	"${CMAKE_CURRENT_BINARY_DIR};${CMAKE_PROJECT_NAME};ALL;/"
# 	"d:\\\\GIT\\\\mc12101_bsp_windows\\\\build;mc12101load;ALL;mc12101load"
# 	)
include(cpack)