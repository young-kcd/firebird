

@echo off

:CHECK_ENV
@if /I "%DB_PATH%"=="" (goto :HELP & goto :EOF) else (@goto :MAIN)

::===========
:MAIN
@echo.
@echo Creating directories
@cd ..\..
@rmdir gen 2>nul
@mkdir gen\alice
@mkdir gen\burp
@mkdir gen\dsql
@mkdir gen\dudley
@mkdir gen\gpre
@mkdir gen\isql
@mkdir gen\jrd
@mkdir gen\msgs
@mkdir gen\qli
@mkdir gen\utilities
@mkdir gen\v5_examples
@cd builds\win32

::provisional, copy all fdbs to the same directory to be used from preprocess
copy dbs\metadata.fdb %ROOT_PATH%\gen\yachts.lnk > nul
copy dbs\jrd\security.fdb %ROOT_PATH%\gen\security.fdb > nul
copy dbs\msgs\msg.fdb %ROOT_PATH%\gen\msg.fdb > nul
copy dbs\qli\help.fdb %ROOT_PATH%\gen\help.fdb > nul

@del ..\..\src\include\gen\autoconfig.h
@copy ..\..\src\include\gen\autoconfig_msvc.h ..\..\src\include\gen\autoconfig.h
@echo Preprocessing files required to do a boot-build...
@call build_boot.bat BOOT
@call preprocess.bat BOOT
@echo.
@echo Making necessary boot-build executables...
@call build_boot.bat
@echo.
@echo Preprocessing the entire source tree...
@call preprocess.bat
@echo Building message file and codes header...
@call build_msg.bat
@echo Building BLR Table
@call blrtable.bat
@goto :NEXT_STEP

::==============
:NEXT_STEP
@echo.
@echo    You may now run makeX_debug.bat or makeX_release.bat
@echo.   where X is your version of MSVC - 6 or 7.
@echo.
@goto :EOF

::==============
:HELP
@echo.
@echo    You must run prepare.bat before running this script
@echo.
@goto :EOF


::==============
:END
