@echo off

@cd msvc6
@set DBG=
@set DBG_DIR=release
@set CLEAN=
@if "%1"=="DEBUG" ((set DBG=TRUE) && (set DBG_DIR=debug))
@if "%2"=="DEBUG" ((set DBG=TRUE) && (set DBG_DIR=debug))
@if "%1"=="CLEAN" (set CLEAN=/REBUILD)
@if "%2"=="CLEAN" (set CLEAN=/REBUILD)
@if "%DBG%"=="" (call :RELEASE) else (call :DEBUG)
@cd ..
@call :MOVE
@goto :END

::===========
:RELEASE
@echo Building...
@msdev Firebird2.dsw /MAKE "fbserver - Win32 Release" "fbguard - Win32 Release" "fb_lock_print - Win32 Release" "fb_inet_server - Win32 Release" "gbak - Win32 Release" "gpre - Win32 Release" "gsplit - Win32 Release" "gdef - Win32 Release" "gfix - Win32 Release" "gsec - Win32 Release" "gstat - Win32 Release" "instreg - Win32 Release" "instsvc - Win32 Release" "isql - Win32 Release" "qli - Win32 Release" "gds32 - Win32 Release" "fbclient - Win32 Release" "ib_udf - Win32 Release" "ib_util - Win32 Release" "intl - Win32 Release" "intlcpp - Win32 Release" %CLEAN% /OUT Firebird2.log
@goto :EOF

::===========
:DEBUG
@echo Building...
@msdev Firebird2.dsw /MAKE "fbserver - Win32 Debug" "fbguard - Win32 Debug" "fb_lock_print - Win32 Debug" "fb_inet_server - Win32 Debug" "gbak - Win32 Debug" "gpre - Win32 Debug" "gsplit - Win32 Debug" "gdef - Win32 Debug" "gfix - Win32 Debug" "gsec - Win32 Debug" "gstat - Win32 Debug" "instreg - Win32 Debug" "instsvc - Win32 Debug" "isql - Win32 Debug" "qli - Win32 Debug" "gds32 - Win32 Debug" "fbclient - Win32 Debug" "ib_udf - Win32 Debug" "ib_util - Win32 Debug" "intl - Win32 Debug" "intlcpp - Win32 Debug" %CLEAN% /OUT Firebird2.log
@goto :EOF

::===========
:MOVE
@echo Copying files to output
@del msvc6\%DBG_DIR%\firebird\bin\*.exp 2>nul
@del msvc6\%DBG_DIR%\firebird\bin\*.lib 2>nul
@rmdir /q /s output 2>nul
@mkdir output
@mkdir output\bin
@mkdir output\intl
@mkdir output\udf
@mkdir output\help
@mkdir output\doc
@mkdir output\include
@mkdir output\lib
@copy msvc6\%DBG_DIR%\firebird\bin\* output\bin >nul
@copy msvc6\%DBG_DIR%\firebird\intl\* output\intl >nul
@copy msvc6\%DBG_DIR%\firebird\udf\* output\udf >nul
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