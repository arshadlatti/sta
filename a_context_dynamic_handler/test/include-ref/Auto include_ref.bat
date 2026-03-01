SET MINGW_PATH_CUR=C:\MinGW\bin
call "%SystemDrive%\mingw_path.bat"
SET PATH=%MINGW_PATH_CUR%;D:\dev\sta-main\tools\include-ref\bin;%PATH%
cls
@echo ******************************************
@echo         * MINGW Build Tools  *
@echo ******************************************
@gcc -o project.exe main.c  -ID:\dev 2>> auto\in_ref.txt
@echo close window or press any key to run project.
@pause
@include_ref auto\auto_inc.h auto\in_ref.txt D:\dev\sta-main\a_gt_list\auto-ref\a_gt_list.txt
@pause

