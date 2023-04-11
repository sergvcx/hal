
	

ifeq ($(OS),Windows_NT)
SHELL=cmd
space := $(subst ,, )
NMC_TOOLPATH=$(subst $(space),\$(space),$(NMC_GCC_TOOLPATH))
-include $(NMC_TOOLPATH)\nmc4-ide\include\nmc4vars_win.mk
#X64_TOOLCHAIN="Visual Studio 15 2017 Win64"
OS_RM    = del 
else 
OS_RM    = rm -f -r
#X64_TOOLCHAIN="Unix Makefiles"
endif

all: mc5103 mc12101 mb7707 mc7601

mc5103:
	cmake --no-warn-unused-cli -B build_$@ . -D HAL_MC5103_BUILD=ON -D CMAKE_TOOLCHAIN_FILE="cmake/nmc-gcc-compile.cmake" -D CMAKE_BUILD_TYPE=Release
	cmake --build build_$@

mc12101:
	cmake --no-warn-unused-cli -B build_$@ . -D HAL_MC12101_BUILD=ON -D CMAKE_TOOLCHAIN_FILE="cmake/nmc-gcc-compile.cmake" -D CMAKE_BUILD_TYPE=Release
	cmake --build build_$@

mb7707:
	cmake --no-warn-unused-cli -B build_$@ . -D HAL_MB7707_BUILD=ON -D CMAKE_TOOLCHAIN_FILE="cmake/nmc-gcc-compile.cmake" -D CMAKE_BUILD_TYPE=Release
	cmake --build build_$@

mc7601:
	cmake --no-warn-unused-cli -B build_$@ . -D HAL_MC7601_BUILD=ON -D CMAKE_TOOLCHAIN_FILE="cmake/nmc-gcc-compile.cmake" -D CMAKE_BUILD_TYPE=Release
	cmake --build build_$@

# x64:
# 	cmake --no-warn-unused-cli -B build_$@ . -D board=$@
# 	cmake --build build_$@

#virtual: