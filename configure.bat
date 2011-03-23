@echo off
@setlocal
cd "%~dp0"

rem where is your cmake install, leave empty if its already in the path
set CMAKE_HOME=
rem which cmake generator do you want to use?
set CMAKE_GENERATOR=Visual Studio 9 2008
rem set CMAKE_GENERATOR=Visual Studio 10

rem enhance the path with cmake
set PATH=%PATH%;%CMAKE_HOME%\bin

rem retrieving directory name for the solution file name
for %%A in ("%~dp0.") do (
	set SOLUTION_NAME=%%~nA
)
echo Directory Name: %SOLUTION_NAME%

rem create build dir and delete existing content
rmdir build
mkdir build
cd build

rem start cmake
echo STARTING CMAKE
cmake -Wno-dev -G "%CMAKE_GENERATOR%" "../"
if %ERRORLEVEL%==0 (
	cmake -Wno-dev "../"
	echo Start solution
	start %SOLUTION_NAME%.sln
) else (
	echo CMake ist not correctly configured, edit this bat file and specify the correct settings or directly use cmake-gui
	pause
)