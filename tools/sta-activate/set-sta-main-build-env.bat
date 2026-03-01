@SET STA_MAIN_PATH=d:\dev

@if exist c:\set-sta-path.bat (
 @call c:\set-sta-path.bat
 @echo found set-sta-path.bat in C
 @goto finished
)

@if exist d:\set-sta-path.bat (
 @call d:\set-sta-path.bat
 @echo found set-sta-path.bat in D
 @goto finished
)

@if exist e:\set-sta-path.bat (
 @call e:\set-sta-path.bat
 @echo found set-sta-path.bat in E
 @goto finished
)

@if exist f:\set-sta-path.bat (
 @call f:\set-sta-path.bat
 @echo found set-sta-path.bat in F
 @goto finished
)

@if exist g:\set-sta-path.bat (
 @call g:\set-sta-path.bat
 @echo found set-sta-path.bat in G
 @goto finished
)

@echo please run D:\dev\sta-main\tools\sta-activate\sta-activate.bat



:finished
@echo sta dir = %STA_MAIN_PATH% 
@pause
