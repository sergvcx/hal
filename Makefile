
ifeq ($(OS),Windows_NT)
include("$(NMC_GCC_TOOLPATH)\nmc4-ide\nmc4vars_win.mk")
NMC_TOOLCHAIN="Ninja"
X64_TOOLCHAIN="Visual Studio 15 2017 Win64"
else
#NMC_TOOLCHAIN="Unix Makefiles"
NMC_TOOLCHAIN="Ninja"
X64_TOOLCHAIN="Unix Makefiles"
endif


mc12101:
	cmake -S . --preset=hal-mc12101
	cmake --build build/hal-mc12101
	cmake -S . --preset=hal-mc12101-host-msvc
	cmake --build build/hal-mc12101-host-msvc

mb7707:
	cmake -S . --preset=hal-mb7707
	cmake --build build/hal-mb7707
	cmake -S . --preset=hal-mb7707-host-msvc
	cmake --build build/hal-mb7707-host-msvc

mc5103:
	cmake -S . --preset=hal-mc5103
	cmake --build build/hal-mc5103
	cmake -S . --preset=hal-mc5103-host-msvc
	cmake --build build/hal-mc5103-host-msvc

mc7601: 
	cmake -S . --preset=hal-mc7601
	cmake --build build/hal-mc7601
	cmake -S . --preset=hal-mc7601-host-msvc
	cmake --build build/hal-mc7601-host-msvc

virtual: 
	cmake -S . --preset=hal-virtual-msvc
	cmake --build build/hal-virtual-msvc	

pack:
	cmake -S target  -B build_pack/target/release  	-D HAL_MC12101=ON -D HAL_MB7707=ON -D HAL_MC7601=ON -D HAL_MC5103=ON -D CMAKE_BUILD_TYPE=Release -G $(NMC_TOOLCHAIN)
	cmake -S target  -B build_pack/target/debug  	-D HAL_MC12101=ON -D HAL_MB7707=ON -D HAL_MC7601=ON -D HAL_MC5103=ON -D CMAKE_BUILD_TYPE=Debug   -G $(NMC_TOOLCHAIN)
	cmake -S host    -B build_pack/host/release    	-D HAL_MC12101=ON -D HAL_MB7707=ON -D HAL_MC7601=ON -D HAL_MC5103=ON -D CMAKE_BUILD_TYPE=Release -G $(NMC_TOOLCHAIN)
	cmake -S host    -B build_pack/host/debug    	-D HAL_MC12101=ON -D HAL_MB7707=ON -D HAL_MC7601=ON -D HAL_MC5103=ON -D CMAKE_BUILD_TYPE=Debug   -G $(NMC_TOOLCHAIN)
ifeq ($(OS),Windows_NT)
	cmake -S virtual -B build_pack/virtual -G $(X64_TOOLCHAIN)
else
#	cmake -S virtual -B build_pack/virtual/release 	-D CMAKE_BUILD_TYPE=Release -G $(X64_TOOLCHAIN)
#	cmake -S virtual -B build_pack/virtual/debug	-D CMAKE_BUILD_TYPE=Debug 	-G $(X64_TOOLCHAIN)
endif
	cmake --build build_pack/target/release
	cmake --build build_pack/target/debug
	cmake --build build_pack/host/release
	cmake --build build_pack/host/debug
ifeq ($(OS),Windows_NT)
	cmake --build build_pack/virtual --config Release
	cmake --build build_pack/virtual --config Debug
else
#	cmake --build build_pack/virtual/release
#	cmake --build build_pack/virtual/debug
endif
	cmake -S . -B build_pack/pack -D HAL_PACK=ON -G $(NMC_TOOLCHAIN)
	cpack --config build_pack/pack/CPackConfig.cmake -G 7Z
	$(info all done)
