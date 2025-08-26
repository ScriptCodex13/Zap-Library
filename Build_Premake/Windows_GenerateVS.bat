@echo off
setlocal ENABLEDELAYEDEXPANSION
@REM the newly created folder is ignored by premake tool
@REM mkdir "..\Project"
mkdir Project
premake5.exe vs2022
endlocal
PAUSE

