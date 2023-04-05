@rd /S /Q converted
@md converted

@echo Converting BIG IDEA..
@cd source
@for /D %%G in (*.) do @call :CONVERT_BIG_IDEA "%%G" "%%~dpG" "%%~nG.log" "%%~nG"
@cd ..





@exit /B 0


:CONVERT_BIG_IDEA
@set Input_Dir=%2\%4
set Input_Dir=%Input_Dir:"=%

@set Output_File=%2..\converted\%3
set Output_File=%Output_File:"=%

@echo Converting BIG IDEA patient: %Input_Dir%
@echo Producing log file: %Output_File%


@console3 "..\config\1_convert_all_signals.ini" --execute

@exit /B 0

