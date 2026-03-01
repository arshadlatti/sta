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
@gcc -o .\bin\include_ref.exe main.c .\project\tmp\project.o -I%STA_PATH_CUR%
@echo close window or press any key to run project.
@pause
rem @.\bin\project.exe
@cd bin
@cd test
@..\include_ref.exe out.h linker.txt ref.txt
@pause

