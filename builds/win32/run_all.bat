
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

call clean_all
call prepare
call make_boot
call make_all
call make_examples
cd ..\install\arch-specific\win32
call BuildExecutableInstall
cd ..\..\..\win32

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
