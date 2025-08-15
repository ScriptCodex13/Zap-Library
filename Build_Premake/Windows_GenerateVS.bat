@echo off
setlocal ENABLEDELAYEDEXPANSION
cd ..
mkdir Project
cd ./Build_Premake/
set /p version="Visual Studio Version:"
premake5.exe %version%

PAUSE

