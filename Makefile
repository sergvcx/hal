
ifeq($(OS),Windows_NT)
include("$(NMC_GCC_TOOLPATH)\nmc4-ide\nmc4vars_win.mk")
endif
# export GCC_EXEC_PREFIX=/cygdrive/C/Program Files/Module/NMC-SDK/nmc4-ide/lib/gcc/
# $(info  $(GCC_EXEC_PREFIX))
# PATH:=$(NMC_GCC_TOOLPATH)\nmc4-ide\bin;$(NMC_GCC_TOOLPATH)\nmc4-ide\lib;$(PATH)



all:
	cmake -B build_all . -D MC12101_BUILD=ON -D MC7601_BUILD=ON -D MC5103_BUILD=ON -D MC7601_BUILD=ON -D VIRTUAL_BUILD=ON
	cmake --build build_all

mc12101:
	cmake -B build_$@ . -D MC12101_BUILD=ON
	cmake --build build_$@

mb7707:
	cmake -B build_$@ . -D MC7601_BUILD=ON
	cmake --build build_$@

mc5103:
	cmake -B build_$@ . -D MC5103_BUILD=ON
	cmake --build build_$@

mc7601: 
	cmake -B build_$@ . -D MC7601_BUILD=ON
	cmake --build build_$@

virtual: 
	cmake -B build_$@ . -D VIRTUAl_BUILD=ON
	cmake --build build_$@
