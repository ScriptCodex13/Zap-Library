@echo off
setlocal ENABLEDELAYEDEXPANSION
mkdir "..\Project"
set /p version="Visual Studio Version:"
premake5.exe vs%version%

PAUSE






