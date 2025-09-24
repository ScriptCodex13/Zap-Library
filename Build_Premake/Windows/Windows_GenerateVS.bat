@echo off
setlocal ENABLEDELAYEDEXPANSION
@REM the newly created folder is ignored by premake tool
@REM mkdir "..\Project"
@set resource_dir=..\..\resource\
@set projects_dir=.\Project\
@set application_dir=%projects_dir%\Application\
mkdir %projects_dir%
premake5.exe vs2022
xcopy /S /I %resource_dir%shader   %application_dir%shader
xcopy /S /I %resource_dir%textures %application_dir%textures
endlocal
PAUSE



