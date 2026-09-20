SET MINGW_PATH_CUR=C:\w64devkit\bin
SET STA_PATH_CUR=D:\dev

SET PATH=%MINGW_PATH_CUR%;%PATH%

cls
@echo ******************************************
@echo         * MINGW Build Tools  *
@echo ******************************************
@cd library
@%STA_PATH_CUR%\sta-main\tools\pabs\bin\pabs.exe ..\generated\library.h ..\generated\library.c
@cd ..
@%STA_PATH_CUR%\sta-main\tools\pabs-depend\bin\pabs-depend.exe %STA_PATH_CUR%\sta-main\pabs sta.depend.txt .\generated\sta_depend.h .\generated\sta_depend.c .\generated\sta_link.bat .\generated\sta_runtime.bat
@%STA_PATH_CUR%\sta-main\tools\pabs-depend\bin\pabs-depend.exe %STA_PATH_CUR%\pabs shared.depend.txt .\generated\shared_depend.h .\generated\shared_depend.c .\generated\shared_link.bat .\generated\shared_runtime.bat
@call .\generated\shared_link.bat
@gcc -std=c99 -I.\library -o .\generated\project.o -c project.c %PABS_LINK% -I%STA_PATH_CUR% -flto -fdata-sections -ffunction-sections
@pause