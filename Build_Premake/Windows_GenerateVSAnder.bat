echo off

@rem Unpack third party libraries for usage
@set cpwd=%cd%
@cd ../3rdparty
@rem don't violate the drive, launch extract_libs.cmd, mmanually only when needed
@rem call extract_libs.cmd
@cd %cpwd%

setlocal ENABLEDELAYEDEXPANSION

@rem Create MSVC projects according to rules written in premake5.ander.lua
@echo start premake
premake5.exe vs2022 --file=premake5.ander.lua
@echo end premake
endlocal
PAUSE
