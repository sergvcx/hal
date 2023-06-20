ifeq ($(OS),Windows_NT)
SHELL=cmd
space := $(subst ,, )
NMC_TOOLPATH=$(subst $(space),\$(space),$(NMC_GCC_TOOLPATH))
X64_PLATFORM=-A\ x64
#-include $(NMC_TOOLPATH)\nmc4-ide\include\nmc4vars_win.mk
NMC_TOOLCHAIN="Ninja"
else 
NMC_TOOLCHAIN="Ninja"
X64_PLATFORM="-G Ninja Multi-Config"
endif

.DEFAULT_GOAL := mytarget
.PHONY: virtual

mc12101:
	cmake -S target -B build/$@/target/release -D HAL_MC12101=ON -D CMAKE_BUILD_TYPE=Release -G $(NMC_TOOLCHAIN)
	cmake -S target -B build/$@/target/debug   -D HAL_MC12101=ON -D CMAKE_BUILD_TYPE=Debug -G $(NMC_TOOLCHAIN)
	cmake --build build/$@/target/release
	cmake --build build/$@/target/debug
	cmake -S host -B build/$@/host -D HAL_MC12101=ON $(X64_PLATFORM)
	cmake --build build/$@/host --config Release
	cmake --build build/$@/host --config Debug

mb7707:
	cmake -S target -B build/$@/target/release -D HAL_MB7707=ON -D CMAKE_BUILD_TYPE=Release -G $(NMC_TOOLCHAIN)
	cmake -S target -B build/$@/target/debug   -D HAL_MB7707=ON -D CMAKE_BUILD_TYPE=Debug -G $(NMC_TOOLCHAIN)
	cmake --build build/$@/target/release
	cmake --build build/$@/target/debug
	cmake -S host -B build/$@/host -D HAL_MB7707=ON $(X64_PLATFORM)
	cmake --build build/$@/host --config Release
	cmake --build build/$@/host --config Debug

mc5103:
	cmake -S target -B build/$@/target/release -D HAL_MC5103=ON -D CMAKE_BUILD_TYPE=Release -G $(NMC_TOOLCHAIN)
	cmake -S target -B build/$@/target/debug   -D HAL_MC5103=ON -D CMAKE_BUILD_TYPE=Debug -G $(NMC_TOOLCHAIN)
	cmake --build build/$@/target/release
	cmake --build build/$@/target/debug
	cmake -S host -B build/$@/host -D HAL_MC5103=ON $(X64_PLATFORM)
	cmake --build build/$@/host --config Release
	cmake --build build/$@/host --config Debug

mc7601: 
	cmake -S target -B build/$@/target/release -D HAL_MC7601=ON -D CMAKE_BUILD_TYPE=Release -G $(NMC_TOOLCHAIN)
	cmake -S target -B build/$@/target/debug   -D HAL_MC7601=ON -D CMAKE_BUILD_TYPE=Debug -G $(NMC_TOOLCHAIN)
	cmake --build build/$@/target/release
	cmake --build build/$@/target/debug
	cmake -S host -B build/$@/host -D HAL_MC7601=ON $(X64_PLATFORM)
	cmake --build build/$@/host --config Release
	cmake --build build/$@/host --config Debug

virtual: 
	cmake -S virtual -B build/$@/virtual $(X64_PLATFORM)
	cmake --build build/$@/virtual --config Release
	cmake --build build/$@/virtual --config Debug


pack-mc12101: mc12101
	cmake -S . -B build/pack -D HAL_PACK=ON -D HAL_MC12101=ON $(X64_PLATFORM) -D CPACK_PACKAGE_NAME=hal-mc12101
	cpack --config build/pack/CPackConfig.cmake -G 7Z -C "Debug;Release"

pack-mb7707: mb7707
	cmake -S . -B build/pack -D HAL_PACK=ON -D HAL_MB7707=ON  $(X64_PLATFORM) -D CPACK_PACKAGE_NAME=hal-mb7707
	cpack --config build/pack/CPackConfig.cmake -G 7Z -C "Debug;Release"

pack-mc5103: mc5103
	cmake -S . -B build/pack -D HAL_PACK=ON -D HAL_MC5103=ON  $(X64_PLATFORM) -D CPACK_PACKAGE_NAME=hal-mc5103
	cpack --config build/pack/CPackConfig.cmake -G 7Z -C "Debug;Release"

pack-mc7601: mc7601
	cmake -S . -B build/pack -D HAL_PACK=ON -D HAL_MC7601=ON  $(X64_PLATFORM) -D CPACK_PACKAGE_NAME=hal-mc7601
	cpack --config build/pack/CPackConfig.cmake -G 7Z -C "Debug;Release"

pack-all: mc12101 mb7707 mc5103 mc7601
	cmake -S . -B build/pack -D HAL_PACK=ON -D HAL_MC12101=ON -D HAL_MB7707=ON -D HAL_MC7601=ON -D HAL_MC5103=ON $(X64_PLATFORM) 
	cpack --config build/pack/CPackConfig.cmake -G 7Z -C "Debug;Release"

clean:
	rm -r build lib
