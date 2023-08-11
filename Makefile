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

setenv:
	setx HAL "%CD%"
	
mc12101:
	cmake --preset mc12101
	cmake --build build/mc12101

mb7707:
	cmake --preset mb7707
	cmake --build build/mb7707

mc5103:
	cmake --preset mc5103
	cmake --build build/mc5103

mc7601: 
	cmake --preset mc7601
	cmake --build build/mc7601

x86: 
	cmake --preset x86-msvc
	cmake --build build/x86-msvc



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
