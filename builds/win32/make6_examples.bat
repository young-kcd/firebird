
@echo off

@if /I "%DB_PATH%"=="" (goto :HELP & goto :EOF) else (@goto :MAIN)

::===========
:: Read input values
@set DBG=
@set DBG_DIR=release
@set CLEAN=
@if "%1"=="DEBUG" ((set DBG=TRUE) && (set DBG_DIR=debug))
@if "%2"=="DEBUG" ((set DBG=TRUE) && (set DBG_DIR=debug))
@if "%1"=="CLEAN" (set CLEAN=/REBUILD)
@if "%2"=="CLEAN" (set CLEAN=/REBUILD)

::===========
:MAIN
@call :BUILD_EMPBUILD
@cd msvc6
@if "%DBG%"=="" (call :RELEASE) else (call :DEBUG)
@cd ..
@call :MOVE
@call :BUILD_EMPLOYEE
@goto :END

::===========
:BUILD_EMPBUILD
@echo.
@echo Building empbuild.fdb
@copy ..\..\src\v5_examples\*.e ..\..\gen\v5_examples\ > nul
@copy ..\..\src\v5_examples\*.sql ..\..\gen\v5_examples\ > nul
@copy ..\..\src\v5_examples\*.inp ..\..\gen\v5_examples\ > nul
@cd ..\..\gen\v5_examples

@echo.
@echo Creating empbuild.fdb
@del empbuild.fdb 2>nul
@isql -i empbld.sql

@echo.
@echo preprocessing empbuild.e
@echo path = %DB_PATH%/gen/v5_examples
@..\..\builds\win32\gpre_static -r -m -n -z empbuild.e empbuild.c -b localhost:%DB_PATH%/gen/v5_examples/
cd ..\..\builds\win32
@goto :EOF

::===========
:RELEASE
@echo.
@echo Building release
@msdev Firebird2.dsw /MAKE "v5_examples - Win32 Release" %CLEAN% /OUT v5_examples.log
@goto :EOF

::===========
:DEBUG
@echo.
@echo Building debug
@msdev Firebird2.dsw /MAKE "v5_examples - Win32 Debug" %CLEAN% /OUT v5_examples.log
@goto :EOF


::===========
:MOVE
@echo.
@mkdir output\v5_examples 2> nul
@echo Moving files to output directory
@copy ..\..\src\v5_examples\* output\v5_examples > nul
@copy ..\..\gen\v5_examples\* output\v5_examples > nul
@copy msvc6\%DBG_DIR%\v5_examples\v5_examples.exe output\v5_examples\empbuild.exe > nul
@goto :EOF

::===========
:: only to test if it works
:BUILD_EMPLOYEE
@echo.
@echo Building employee.fdb
@cd output\v5_examples
@empbuild.exe %DB_PATH%\builds\win32\output\v5_examples\employee.fdb
@del %ROOT_PATH%\builds\win32\output\v5_examples\employee.fdb
@cd ..\..
@goto :EOF

::==============
:HELP
@echo.
@echo    You must run prepare.bat before running this script
@echo.
@goto :EOF

:END