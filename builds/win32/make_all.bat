@echo off

:: Set env vars
@call setenvvar.bat
@if errorlevel 1 (goto :END)

:: verify that prepare was run before
@if not exist %ROOT_PATH%\gen\dbs\metadata.fdb (goto :HELP_PREP & goto :END)

:: verify that boot was run before
@if not exist %ROOT_PATH%\gen\gpre_boot.exe (goto :HELP_BOOT & goto :END)

::===========
:: Read input values
@set DBG=
@set DBG_DIR=release
@set CLEAN=/build
@if "%1"=="DEBUG" ((set DBG=TRUE) && (set DBG_DIR=debug))
@if "%2"=="DEBUG" ((set DBG=TRUE) && (set DBG_DIR=debug))
@if "%1"=="CLEAN" (set CLEAN=/rebuild)
@if "%2"=="CLEAN" (set CLEAN=/rebuild)

::==========
:: MAIN
@if "%DBG%"=="" (
	call :RELEASE
) else (
	call :DEBUG
)
@call :MOVE
@goto :END

::===========
:RELEASE
@echo Building release
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2.dsw /MAKE "fbserver - Win32 Release" "fbguard - Win32 Release" "fb_lock_print - Win32 Release" "fb_inet_server - Win32 Release" "gbak - Win32 Release" "gpre - Win32 Release" "gsplit - Win32 Release" "gdef - Win32 Release" "gfix - Win32 Release" "gsec - Win32 Release" "gstat - Win32 Release" "instreg - Win32 Release" "instsvc - Win32 Release" "isql - Win32 Release" "qli - Win32 Release" "gds32 - Win32 Release" "fbclient - Win32 Release" "ib_udf - Win32 Release" "ib_util - Win32 Release" "intl - Win32 Release" "intlcpp - Win32 Release" %CLEAN% /OUT all.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2.sln %CLEAN% release /OUT all.log
)
@goto :EOF

::===========
:DEBUG
@echo Building debug
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2.dsw /MAKE "fbserver - Win32 Debug" "fbguard - Win32 Debug" "fb_lock_print - Win32 Debug" "fb_inet_server - Win32 Debug" "gbak - Win32 Debug" "gpre - Win32 Debug" "gsplit - Win32 Debug" "gdef - Win32 Debug" "gfix - Win32 Debug" "gsec - Win32 Debug" "gstat - Win32 Debug" "instreg - Win32 Debug" "instsvc - Win32 Debug" "isql - Win32 Debug" "qli - Win32 Debug" "gds32 - Win32 Debug" "fbclient - Win32 Debug" "ib_udf - Win32 Debug" "ib_util - Win32 Debug" "intl - Win32 Debug" "intlcpp - Win32 Debug" %CLEAN% /OUT all.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2.sln %CLEAN% debug /OUT all.log
)
@goto :EOF

::===========
:MOVE
@echo Copying files to output
@del %ROOT_PATH%\temp\%DBG_DIR%\firebird\bin\*.exp 2>nul
@del %ROOT_PATH%\temp\%DBG_DIR%\firebird\bin\*.lib 2>nul
@rmdir /q /s %ROOT_PATH%\output 2>nul
@mkdir %ROOT_PATH%\output
@mkdir %ROOT_PATH%\output\bin
@mkdir %ROOT_PATH%\output\intl
@mkdir %ROOT_PATH%\output\udf
@mkdir %ROOT_PATH%\output\help
@mkdir %ROOT_PATH%\output\doc
@mkdir %ROOT_PATH%\output\include
@mkdir %ROOT_PATH%\output\lib
@mkdir %ROOT_PATH%\output\examples
::
@copy %ROOT_PATH%\temp\%DBG_DIR%\firebird\bin\* %ROOT_PATH%\output\bin >nul
@copy %ROOT_PATH%\temp\%DBG_DIR%\firebird\intl\* %ROOT_PATH%\output\intl >nul
@copy %ROOT_PATH%\temp\%DBG_DIR%\firebird\udf\* %ROOT_PATH%\output\udf >nul
:: Firebird.conf, etc
@copy %ROOT_PATH%\gen\firebird.msg %ROOT_PATH%\output > nul
@copy %ROOT_PATH%\builds\install\misc\firebird.conf %ROOT_PATH%\output >nul
:: DATABASES
@copy %ROOT_PATH%\gen\dbs\SECURITY.FDB %ROOT_PATH%\output\security.fdb >nul
@copy %ROOT_PATH%\gen\dbs\HELP.fdb %ROOT_PATH%\output\help\help.fdb >nul
@copy %ROOT_PATH%\gen\firebird.msg %ROOT_PATH%\output\firebird.msg >nul
@copy %ROOT_PATH%\builds\misc\security.gbak %ROOT_PATH%\output\security.fbk > nul
:: LIB
@copy %ROOT_PATH%\temp\%DBG_DIR%\gds32\gds32.lib %ROOT_PATH%\output\lib\gds32_ms.lib >nul
@copy %ROOT_PATH%\temp\%DBG_DIR%\fbclient\fbclient.lib %ROOT_PATH%\output\lib\fbclient_ms.lib >nul
:: DOCS
@copy %ROOT_PATH%\ChangeLog %ROOT_PATH%\output\doc\ChangeLog.txt >nul
@copy %ROOT_PATH%\doc\WhatsNew %ROOT_PATH%\output\doc\WhatsNew.txt >nul
:: HEADERS
:: build headers
copy %ROOT_PATH%\src\misc\gds_header.txt %ROOT_PATH%\output\include\gds.tmp > nul
type %ROOT_PATH%\src\include\fb_types.h >> %ROOT_PATH%\output\include\gds.tmp
type %ROOT_PATH%\src\jrd\ibase.h >> %ROOT_PATH%\output\include\gds.tmp
type %ROOT_PATH%\src\jrd\blr.h >> %ROOT_PATH%\output\include\gds.tmp
type %ROOT_PATH%\src\include\gen\iberror.h >> %ROOT_PATH%\output\include\gds.tmp
type %ROOT_PATH%\src\jrd\gdsold.h >> %ROOT_PATH%\output\include\gds.tmp
type %ROOT_PATH%\src\include\gen\codes.h >> %ROOT_PATH%\output\include\gds.tmp
sed -f %ROOT_PATH%\src\misc\headers.sed < %ROOT_PATH%\output\include\gds.tmp > %ROOT_PATH%\output\include\gds.h
del %ROOT_PATH%\output\include\gds.tmp > nul

copy %ROOT_PATH%\src\misc\ibase_header.txt %ROOT_PATH%\output\include\ibase.tmp > nul
type %ROOT_PATH%\src\include\fb_types.h >> %ROOT_PATH%\output\include\ibase.tmp
type %ROOT_PATH%\src\jrd\ibase.h >> %ROOT_PATH%\output\include\ibase.tmp 
type %ROOT_PATH%\src\jrd\blr.h >> %ROOT_PATH%\output\include\ibase.tmp
type %ROOT_PATH%\src\include\gen\iberror.h >> %ROOT_PATH%\output\include\ibase.tmp
sed -f %ROOT_PATH%\src\misc\headers.sed < %ROOT_PATH%\output\include\ibase.tmp > %ROOT_PATH%\output\include\ibase.h
del %ROOT_PATH%\output\include\ibase.tmp > nul

copy %ROOT_PATH%\src\extlib\ib_util.h %ROOT_PATH%\output\include > nul
copy %ROOT_PATH%\src\jrd\perf.h %ROOT_PATH%\output\include >nul
copy %ROOT_PATH%\src\jrd\blr.h %ROOT_PATH%\output\include > nul
copy %ROOT_PATH%\src\include\gen\iberror.h %ROOT_PATH%\output\include > nul
:: Examples
@xcopy /E %ROOT_PATH%\examples %ROOT_PATH%\output\examples > nul
::
@copy install_super.bat %ROOT_PATH%\output\bin >nul
@copy install_classic.bat %ROOT_PATH%\output\bin >nul
@copy uninstall.bat %ROOT_PATH%\output\bin >nul

@goto :EOF

::==============
:HELP_PREP
@echo.
@echo    You must run prepare.bat before running this script
@echo.
@goto :EOF

::==============
:HELP_BOOT
@echo.
@echo    You must run make_boot.bat before running this script
@echo.
@goto :EOF

:END