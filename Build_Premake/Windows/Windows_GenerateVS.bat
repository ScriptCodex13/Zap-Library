@echo off
setlocal ENABLEDELAYEDEXPANSION
@REM the newly created folder is ignored by premake tool
@REM mkdir "..\Project"
mkdir Project
xcopy /S /I ..\..\shader .\Project\Application\shader
premake5.exe vs2022
xcopy /S /I ..\..\Zap3DDemo\Zap3DTrianglesBasic\textures .\Project\Application\textures
endlocal
PAUSE



