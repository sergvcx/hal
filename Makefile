ifeq ($(OS),Windows_NT)
SHELL=cmd
space := $(subst ,, )
NMC_TOOLPATH=$(subst $(space),\$(space),$(NMC_GCC_TOOLPATH))
-include $(NMC_TOOLPATH)\nmc4-ide\include\nmc4vars_win.mk
#X64_TOOLCHAIN="Visual Studio 15 2017 Win64"
#X64_TOOLCHAIN="Visual Studio 16 2019"
NMC_TOOLCHAIN="Ninja"
else 
X64_TOOLCHAIN="Ninja Multi-Config"
NMC_TOOLCHAIN="Ninja"
endif

.DEFAULT_GOAL := mytarget
.PHONY: virtual

mc12101:
	cmake -S target -B build/$@/target/release -D HAL_MC12101=ON -D CMAKE_BUILD_TYPE=Release -G $(NMC_TOOLCHAIN)
	cmake -S target -B build/$@/target/debug   -D HAL_MC12101=ON -D CMAKE_BUILD_TYPE=Debug -G $(NMC_TOOLCHAIN)
	cmake --build build/$@/target/release
	cmake --build build/$@/target/debug
	cmake -S host -B build/$@/host -D HAL_MC12101=ON -A x64 
	cmake --build build/$@/host --config Release
	cmake --build build/$@/host --config Debug

mb7707:
	cmake -S target -B build/$@/target/release -D HAL_MB7707=ON -D CMAKE_BUILD_TYPE=Release -G $(NMC_TOOLCHAIN)
	cmake -S target -B build/$@/target/debug   -D HAL_MB7707=ON -D CMAKE_BUILD_TYPE=Debug -G $(NMC_TOOLCHAIN)
	cmake --build build/$@/target/release
	cmake --build build/$@/target/debug
	cmake -S host -B build/$@/host -D HAL_MB7707=ON -A x64
	cmake --build build/$@/host --config Release
	cmake --build build/$@/host --config Debug

mc5103:
	cmake -S target -B build/$@/target/release -D HAL_MC5103=ON -D CMAKE_BUILD_TYPE=Release -G $(NMC_TOOLCHAIN)
	cmake -S target -B build/$@/target/debug   -D HAL_MC5103=ON -D CMAKE_BUILD_TYPE=Debug -G $(NMC_TOOLCHAIN)
	cmake --build build/$@/target/release
	cmake --build build/$@/target/debug
	cmake -S host -B build/$@/host -D HAL_MC5103=ON -A x64
	cmake --build build/$@/host --config Release
	cmake --build build/$@/host --config Debug

mc7601: 
	cmake -S target -B build/$@/target/release -D HAL_MC7601=ON -D CMAKE_BUILD_TYPE=Release -G $(NMC_TOOLCHAIN)
	cmake -S target -B build/$@/target/debug   -D HAL_MC7601=ON -D CMAKE_BUILD_TYPE=Debug -G $(NMC_TOOLCHAIN)
	cmake --build build/$@/target/release
	cmake --build build/$@/target/debug
	cmake -S host -B build/$@/host -D HAL_MC7601=ON -A x64
	cmake --build build/$@/host --config Release
	cmake --build build/$@/host --config Debug

virtual: 
	cmake -S virtual -B build/$@/virtual -A x64
	cmake --build build/$@/virtual --config Release
	cmake --build build/$@/virtual --config Debug


pack-mc12101: mc12101
	cmake -S . -B build/pack -D HAL_PACK=ON -A x64 -D HAL_MC12101=ON
	cpack --config build/pack/CPackConfig.cmake -G 7Z -C "Debug;Release"

pack-mb7707: mb7707
	cmake -S . -B build/pack -D HAL_PACK=ON -A x64 -D HAL_MB7707=ON
	cpack --config build/pack/CPackConfig.cmake -G 7Z -C "Debug;Release"

pack-mc5103: mc5103
	cmake -S . -B build/pack -D HAL_PACK=ON -A x64 -D HAL_MC5103=ON
	cpack --config build/pack/CPackConfig.cmake -G 7Z -C "Debug;Release"

pack-mc7601: mc7601
	cmake -S . -B build/pack -D HAL_PACK=ON -A x64 -D HAL_MC7601=ON
	cpack --config build/pack/CPackConfig.cmake -G 7Z -C "Debug;Release"

pack-all: mc12101 mb7707 mc5103 mc7601
	cmake -S . -B build/pack -D HAL_PACK=ON -A x64 -D HAL_MC12101=ON -D HAL_MB7707=ON -D HAL_MC7601=ON -D HAL_MC5103=ON
	cpack --config build/pack/CPackConfig.cmake -G 7Z -C "Debug;Release"

clean:
	rm -r build lib
