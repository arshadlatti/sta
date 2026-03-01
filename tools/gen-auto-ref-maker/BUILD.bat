SET MINGW_PATH_CUR=C:\MinGW\bin
SET STA_PATH_CUR=D:\dev


call "%SystemDrive%\mingw_path.bat"
call ..\mingw_path.bat
SET PATH=%MINGW_PATH_CUR%;%PATH%

cls
@echo ******************************************
@echo         * MINGW Build Tools  *
@echo ******************************************
@REM @echo %STA_PATH_CUR%
@gcc -o .\bin\get-auto-ref-maker.exe main.c .\project\tmp\project.o -I%STA_PATH_CUR%
@echo close window or press any key to run project.
@pause
@bin\get-auto-ref-maker.exe a_lib_name
@pause

