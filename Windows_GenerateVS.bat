@echo off
setlocal ENABLEDELAYEDEXPANSION
mkdir Project
set /p version="Visual Studio Version:"
echo %version%
premake5.exe %version%
PAUSE