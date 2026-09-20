SET MINGW_PATH_CUR=C:\MinGW\bin
SET STA_PATH_CUR=D:\dev

SET PATH=%MINGW_PATH_CUR%;%PATH%

cls
@echo ******************************************
@echo         * MINGW Build Tools  *
@echo ******************************************
@cd library
@%STA_PATH_CUR%\sta-main\tools\pabs\bin\pabs.exe ..\generated\library.h ..\generated\library.c
@cd ..
@gcc -std=c99 -I.\library -o .\generated\project.o -c project.c -I%STA_PATH_CUR% -flto -fdata-sections -ffunction-sections
@pause