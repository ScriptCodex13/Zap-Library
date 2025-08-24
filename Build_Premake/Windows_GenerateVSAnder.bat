echo off
setlocal ENABLEDELAYEDEXPANSION
@REM the newly created folder is ignored by premake tool
@REM mkdir "..\Project"
@echo start premake
premake5.exe vs2022 --file=premake5.ander.lua
@echo end premake
endlocal
@cd ../3rdparty
@extract_libs.cmd
PAUSE
