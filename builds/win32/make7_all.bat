@echo off

@cd msvc7
@set DBG=
@set DBG_DIR=release
@set CLEAN=/build
@if "%1"=="DEBUG" ((set DBG=TRUE) && (set DBG_DIR=debug))
@if "%2"=="DEBUG" ((set DBG=TRUE) && (set DBG_DIR=debug))
@if "%1"=="CLEAN" (set CLEAN=/clean)
@if "%2"=="CLEAN" (set CLEAN=/clean)
@if "%DBG%"=="" (call :RELEASE) else (call :DEBUG)
@cd ..
@call :MOVE
@goto :END

::===========
:RELEASE
@echo Building...
devenv Firebird2.sln %CLEAN% /OUT Firebird2.log
@goto :EOF

::===========
:DEBUG
@echo Building...
devenv Firebird2.sln %CLEAN% debug /OUT Firebird2.log
@goto :EOF

::===========
:MOVE
@echo Copying files to output
@del msvc7\%DBG_DIR%\firebird\bin\*.exp 2>nul
@del msvc7\%DBG_DIR%\firebird\bin\*.lib 2>nul
@rmdir /q /s output 2>nul
@mkdir output
@mkdir output\bin
@mkdir output\intl
@mkdir output\udf
@mkdir output\help
@mkdir output\doc
@mkdir output\include
@mkdir output\lib
@copy msvc7\%DBG_DIR%\firebird\bin\* output\bin >nul
@copy msvc7\%DBG_DIR%\firebird\intl\* output\intl >nul
@copy msvc7\%DBG_DIR%\firebird\udf\* output\udf >nul
@copy gpre.exe output\bin >nul
@copy dbs\jrd\SECURITY.FDB output\security.fdb >nul
@copy dbs\qli\HELP.fdb output\help\help.fdb >nul
@copy ..\..\ChangeLog output\doc\ChangeLog.txt >nul
@copy ..\..\doc\WhatsNew output\doc\WhatsNew.txt >nul
@copy ..\..\src\jrd\ibase.h output\include >nul
@copy ..\..\src\include\gen\iberror.h output\include >nul
@copy install_super.bat output\bin >nul
@copy install_classic.bat output\bin >nul
@copy uninstall.bat output\bin >nul

@goto :EOF

:END