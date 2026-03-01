SET MINGW_PATH_CUR=C:\MinGW\bin
call "%SystemDrive%\mingw_path.bat"
SET PATH=%MINGW_PATH_CUR%;%PATH%
cls
@echo ******************************************
@echo         * MINGW Build Tools  *
@echo ******************************************
@gcc -o project.exe main.c 
@echo close window or press any key to run project.
@pause
@project
@pause

