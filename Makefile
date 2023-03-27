
# ifeq($(OS),Windows_NT)
# include("$(NMC_GCC_TOOLPATH)\nmc4-ide\nmc4vars_win.mk")
# endif
# export GCC_EXEC_PREFIX=/cygdrive/C/Program Files/Module/NMC-SDK/nmc4-ide/lib/gcc/
# $(info  $(GCC_EXEC_PREFIX))
# PATH:=$(NMC_GCC_TOOLPATH)\nmc4-ide\bin;$(NMC_GCC_TOOLPATH)\nmc4-ide\lib;$(PATH)




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
