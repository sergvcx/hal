@call cleanup.bat 1> nul 2> nul

@setlocal

@call "%NMC_GCC_TOOLPATH%\nmc4cmd.bat"
popd

@echo on

nmc-g++ -o simple.abs simple.cpp ^
-I ..\..\include -I "%MC12101%\include"  ^
-Wl,--whole-archive -l mc12101load_nm -Wl,--no-whole-archive  ^
-L ..\..\lib -L "%MC12101%\lib" -L "%NMC_GCC_TOOLPATH%\nmc4-ide\lib" ^
-T mc12101brd.lds

nmc-g++ -o host.abs host.cpp ^
-I ..\..\include -I "%MC12101%\include"  ^
-Wl,--whole-archive -l mc12101load_nm -lnm6407_io_nmc -Wl,--no-whole-archive -lhal_mc12101d -Xlinker -Map="map.txt"  ^
-L ..\..\lib -L "%MC12101%\lib" -L "%NMC_GCC_TOOLPATH%\nmc4-ide\lib" ^
-T mc12101brd.lds

@echo off

endlocal
