
@echo off

@if "%1" NEQ "" (set FIREBIRD=%1)
:: BRS 
:: Get all the file name when there are spaces
:: this can be also achieved with %* but I don't know which versions of 
:: windows allows it
:LOOP
	@shift
	@if "%1"==""  (goto DONE)
        @set FIREBIRD=%FIREBIRD% %1
	@goto loop
:DONE

@echo      FIREBIRD=%FIREBIRD%
@if "%FIREBIRD%"=="" (goto :HELP & goto :EOF)

set ERRLEV=0

call clean_all
call prepare
:: call parse
call make_boot 
if "%ERRLEV%"=="1" goto :END2
call make_all 
if "%ERRLEV%"=="1" goto :END2
call make_examples
if "%ERRLEV%"=="1" goto :END2
cd ..\install\arch-specific\win32
call BuildExecutableInstall
cd ..\..\..\win32

goto:END2

::===========
:HELP
@echo.
@echo   Build process need the FIREBIRD environment variable set to work.
@echo   FIREBIRD value should be the root directory of your Firebird installation.
@echo   Example:
@echo     c:\program files\firebird
@echo. 
@goto :END2

:END2
