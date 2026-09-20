SET MINGW_PATH_CUR=C:\MinGW\bin
SET STA_PATH_CUR=D:\dev

SET PATH=%MINGW_PATH_CUR%;%PATH%

cls
@echo ******************************************
@echo         * MINGW Build Tools  *
@echo ******************************************

@gcc -std=c99 -I%STA_PATH_CUR% -I.\project\library -o .\bin\project.exe main.c .\project\generated\project.o -flto -fdata-sections -ffunction-sections -Wl,-gc-sections -Wl,-strip-all 
@echo close window or press any key to run project.
@pause
@cd bin
@cd test
@..\project.exe
@pause

