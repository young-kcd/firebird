

@echo off

:CHECK_ENV
@if /I "%DB_PATH%"=="" (goto :HELP & goto :EOF) else (@goto :MAIN)

::===========
:MAIN
@echo.
@del ..\..\src\include\gen\autoconfig.h 2> nul
@copy ..\..\src\include\gen\autoconfig_msvc.h ..\..\src\include\gen\autoconfig.h > nul
@echo Preprocessing files required to do a boot-build...
@cd msvc6
@msdev Firebird2Boot.dsw /MAKE "common_static - Win32 Release" "gpre_boot - Win32 Release"  /REBUILD /OUT boot1.log
@cd ..
@del gpre_boot.exe 2> nul
@move msvc6\release\firebird\bin\gpre_boot.exe .
@call preprocess.bat BOOT

@echo.
@echo Making necessary boot-build executables...
@cd msvc6
@msdev Firebird2Boot.dsw /MAKE "fbclient_static - Win32 Release" "gpre_static - Win32 Release"  /REBUILD /OUT boot2.log
@cd ..
@del gpre_static.exe 2> nul
@move msvc6\release\firebird\bin\gpre_static.exe .

@echo Preprocessing the entire source tree...
@call preprocess.bat
@echo Building message file and codes header...
@cd msvc6
@msdev Firebird2Boot.dsw /MAKE "build_msg - Win32 Release" "codes - Win32 Release"  /REBUILD /OUT boot3.log
@cd ..
@del build_msg.exe 2> nul
@move msvc6\release\build_msg\build_msg.exe .
@del codes.exe 2> nul
@move msvc6\release\codes\codes.exe .
@build_msg -D %DB_PATH%/generated/msg.fdb
@codes %ROOT_PATH%\src\include\gen
@echo Building BLR Table
@call blrtable.bat
@goto :NEXT_STEP

::==============
:NEXT_STEP
@echo.
@echo    You may now run make6_debug.bat or make6_release.bat
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
