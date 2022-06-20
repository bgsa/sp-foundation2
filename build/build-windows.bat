@echo off 

SET BUILD_DIR=output
SET SHARED_LIB=ON

if exist %BUILD_DIR% ( rmdir /s/q %BUILD_DIR% )
mkdir %BUILD_DIR%
cd %BUILD_DIR%

cmake ..\..\ -G "Visual Studio 17 2022" -A x64 ^
	-DOPERATING_SYSTEM:STRING=WINDOWS          ^
	-DARCH:STRING=x86_64                       ^
	-DBUILD_SHARED_LIBS:BOOL=%SHARED_LIB%      ^
	-DCMAKE_BUILD_TYPE:STRING=Debug            ^
	-DFFMPEG_ENABLED:BOOL=ON                   ^
	-DCMAKE_ENABLE_EXPORTS:BOOL=ON

cmake --build . --config Debug

cd ..\
if exist %BUILD_DIR% ( rmdir /s/q %BUILD_DIR% )



if exist %BUILD_DIR% ( rmdir /s/q %BUILD_DIR% )
mkdir %BUILD_DIR%
cd %BUILD_DIR%

cmake ..\..\ -G "Visual Studio 17 2022" -A x64 ^
	-DOPERATING_SYSTEM:STRING=WINDOWS          ^
	-DARCH:STRING=x86_64                       ^
	-DBUILD_SHARED_LIBS:BOOL=%SHARED_LIB%      ^
	-DCMAKE_BUILD_TYPE:STRING=Release          ^
	-DFFMPEG_ENABLED:BOOL=ON                   ^
	-DCMAKE_ENABLE_EXPORTS:BOOL=ON

cmake --build . --config Release

cd ..\
if exist %BUILD_DIR% ( rmdir /s/q %BUILD_DIR% )