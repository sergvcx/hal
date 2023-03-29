include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR})
configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/hal-config.cmake.in hal-config.cmake
			INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hal
			PATH_VARS INSTALL_LIBDIR)
write_basic_package_version_file(hal-config-version.cmake
			COMPATIBILITY SameMajorVersion)
install(FILES
			"${CMAKE_CURRENT_BINARY_DIR}/hal-config.cmake"
			"${CMAKE_CURRENT_BINARY_DIR}/hal-config-version.cmake"
			COMPONENT hal-dev
			DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/hal")

set(CPACK_PACKAGE_NAME hal)
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/..)
set(CPACK_NSIS_INSTALL_ROOT "C:\\\\Program Files\\\\Module")
set(CPACK_PACKAGE_VENDOR "RC Module")

if(UNIX)
set(CPACK_INSTALL_CMAKE_PROJECTS
    "${CMAKE_CURRENT_LIST_DIR}/../build_pack/target/release;hal-target;ALL;/"
	"${CMAKE_CURRENT_LIST_DIR}/../build_pack/target/debug;hal-target;ALL;/" 
	"${CMAKE_CURRENT_LIST_DIR}/../build_pack/host/release;hal-host;ALL;/"
    "${CMAKE_CURRENT_LIST_DIR}/../build_pack/host/debug;hal-host;ALL;/"
	"${CMAKE_CURRENT_LIST_DIR}/../build_pack/pack;hal;ALL;/"
    #  "${CMAKE_CURRENT_LIST_DIR}/../build_pack/virtual/release;hal-virtual;ALL;/"
    #  "${CMAKE_CURRENT_LIST_DIR}/../build_pack/virtual/debug;hal-virtual;ALL;/"
	)
else()
set(CPACK_INSTALL_CMAKE_PROJECTS
	"${CMAKE_CURRENT_LIST_DIR}/../build_pack/target/release;hal-target;ALL;/"
	"${CMAKE_CURRENT_LIST_DIR}/../build_pack/target/debug;hal-target;ALL;/" 
	"${CMAKE_CURRENT_LIST_DIR}/../build_pack/host;hal-host;ALL;/"    
	"${CMAKE_CURRENT_LIST_DIR}/../build_pack/pack;hal;ALL;/"
    #  "${CMAKE_CURRENT_LIST_DIR}/../build_pack/virtual;hal-virtual;ALL;/"
	)
endif()
include(CPack)