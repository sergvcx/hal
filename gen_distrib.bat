set NMC_TOOLCHAIN=Ninja
set X64_TOOLCHAIN="Visual Studio 16 2019"
call "%NMC_GCC_TOOLPATH%/nmc4cmd.bat" .
cmake -S target  -B build_pack/target/release  	-D HAL_MC12101=ON -D HAL_MB7707=ON -D HAL_MC7601=ON -D HAL_MC5103=ON -D CMAKE_BUILD_TYPE=Release -G %NMC_TOOLCHAIN%
cmake -S target  -B build_pack/target/debug  	-D HAL_MC12101=ON -D HAL_MB7707=ON -D HAL_MC7601=ON -D HAL_MC5103=ON -D CMAKE_BUILD_TYPE=Debug   -G %NMC_TOOLCHAIN%
cmake --build build_pack/target/release
cmake --build build_pack/target/debug
cmake -S host    -B build_pack/host	-D HAL_MC12101=ON -D HAL_MB7707=ON -D HAL_MC7601=ON -D HAL_MC5103=ON -G %X64_TOOLCHAIN%
cmake --build build_pack/host --config Release
cmake --build build_pack/host --config Debug
rem cmake -S virtual -B build_pack/virtual 	-G %X64_TOOLCHAIN%
rem	cmake --build build_pack/virtual --config Release
rem	cmake --build build_pack/virtual --config Debug
cmake -S . -B build_pack/pack -D HAL_PACK=ON -G %X64_TOOLCHAIN%
cpack --config build_pack/pack/CPackConfig.cmake -G 7Z -C "Debug;Release;
set NMC_TOOLCHAIN=
set X64_TOOLCHAIN=
del /F /Q build_pack