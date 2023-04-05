@rd /S /Q converted
@md converted

@echo Converting BGLP..
@cd source
@for /R %%G in (*.xml) do @call :CONVERT_BGLP "%%G" "%%~dpG" "%%~nG.log" 
@cd ..





@exit /B 0


:CONVERT_BGLP
@set Input_File=%1
@set Input_File=%Input_File:"=%

@set Output_File=%2..\converted\%3
@set Output_File=%Output_File:"=%

@echo Converting BGLP file: %Input_File%
@echo Producing log file: %Output_File%

console3 "..\config\1_convert_all_signals.ini" --execute

@exit /B 0

