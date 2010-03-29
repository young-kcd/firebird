
@echo off

set ERRLEV=0

call clean_all
call make_icu
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
