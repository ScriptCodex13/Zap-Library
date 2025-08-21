@del .\ChromeRoatta.zip /Q
@set zip7zPath=".\7z.exe"
@if not exist %zip7zPath% set zip7zPath="%ProgramFiles%\7-Zip\7z.exe"
@if not exist %zip7zPath% set zip7zPath="%ProgramFiles(x86)%\7-Zip\7z.exe"
@if not exist %zip7zPath% set zip7zPath="7z.exe"
@if not exist %zip7zPath% goto end
rd /s /q glfw-3.4.bin.WIN64 glm-0.9.9.7 stb
@rem goto finish
@%zip7zPath% x .\glfw-3.4.bin.WIN64.zip
@%zip7zPath% x .\glm-0.9.9.7.zip -oglm-0.9.9.7
@%zip7zPath% x .\stb.zip -ostb
@goto finish
:end
@echo unsuccessfull finish, probably 7z not installed
:finish