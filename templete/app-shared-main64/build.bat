SET MINGW_PATH_CUR=C:\w64devkit\bin
SET STA_PATH_CUR=D:\dev

SET PATH=%MINGW_PATH_CUR%;%PATH%

cls
@echo ******************************************
@echo         * MINGW Build Tools  *
@echo ******************************************
@call .\project\generated\shared_link.bat
@gcc -std=c99 -I%STA_PATH_CUR% -I.\project\library -o .\bin\project.exe .\project\app.c %PABS_LINK% .\project\generated\project.o -flto -fdata-sections -ffunction-sections -Wl,-gc-sections -Wl,-strip-all 
@echo close window or press any key to run project.
@pause
@call .\project\generated\shared_runtime.bat
@cd bin
@cd test
@..\project.exe
@pause

