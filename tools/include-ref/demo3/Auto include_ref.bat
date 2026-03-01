SET MINGW_PATH_CUR=C:\MinGW\bin
call "%SystemDrive%\mingw_path.bat"
SET PATH=%MINGW_PATH_CUR%;D:\dev\c3\sta-tool\include-ref\include-ref\bin;%PATH%
cls
@echo ******************************************
@echo         * MINGW Build Tools  *
@echo ******************************************
@gcc -o project.exe main.c 2>> auto\in_ref.txt
@echo close window or press any key to run project.
@pause
@include_ref auto\auto_inc.h auto\in_ref.txt libs\foo_fun_ref.txt libs\baz_ref.txt
@pause

