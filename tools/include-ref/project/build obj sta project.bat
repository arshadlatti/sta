SET MINGW_PATH_CUR=C:\MinGW\bin
SET STA_PATH_CUR=D:\Projects


call "%SystemDrive%\mingw_path.bat"
call ..\mingw_path.bat
SET PATH=%MINGW_PATH_CUR%;%PATH%

cls
@echo ******************************************
@echo         * MINGW Build Tools  *
@echo ******************************************

@gcc -o .\tmp\project.o -c project.c -std=c99 -I%STA_PATH_CUR%
@pause