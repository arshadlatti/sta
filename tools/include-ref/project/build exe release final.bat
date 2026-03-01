SET MINGW_PATH_CUR=C:\MinGW\bin
SET STA_PATH_CUR=D:\Projects


call "%SystemDrive%\mingw_path.bat"
call ..\mingw_path.bat
SET PATH=%MINGW_PATH_CUR%;%PATH%

cls
@echo ******************************************
@echo         * MINGW Build Tools  *
@echo ******************************************
@del tmp\project.o
@gcc -o ..\bin\include_ref.exe ..\main.c -DSTA_PROJECT_FULL -DSTA_PROJECT_FINAL -DSTA_PROJECT_IMPLEMENTATION -std=c99 -I%STA_PATH_CUR% -flto -fdata-sections -ffunction-sections -Wl,-gc-sections -Wl,-strip-all
@pause
