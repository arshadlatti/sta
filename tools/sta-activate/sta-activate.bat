@cd ..
@cd ..
@cd ..
@echo Current Directory = %CD%
@set CurrentDrive=%CD:~0,2%
@echo Current Drive = %CurrentDrive%
@echo SET STA_MAIN_PATH=%CD% > %CurrentDrive%\set-sta-path.bat
@echo trying to create set-sta-path.bat in SystemDrive (can C:)
@echo SET STA_MAIN_PATH=%CD% > %SystemDrive%\set-sta-path.bat
@echo SET STA_MAIN_PATH=%CD% > d:\set-sta-path.bat
@pause
