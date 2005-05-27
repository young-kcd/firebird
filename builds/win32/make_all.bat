@echo off
set ERRLEV=0

:: Set env vars
@call setenvvar.bat
@if errorlevel 1 (call :ERROR Executing setenvvar.bat failed & goto :EOF)

:: verify that prepare was run before
@if not exist %ROOT_PATH%\gen\dbs\metadata.fdb (goto :HELP_PREP & goto :EOF)

:: verify that boot was run before
@if not exist %ROOT_PATH%\gen\gpre_boot.exe (goto :HELP_BOOT & goto :EOF)

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
@if "%ERRLEV%"=="1" (
  @goto :EOF
) else (
  @call :MOVE
)
@goto :EOF

::===========
:RELEASE
@echo Building release
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2.dsw /MAKE "fbserver - Win32 Release" "fbguard - Win32 Release" "fb_lock_print - Win32 Release" "fb_inet_server - Win32 Release" "fbembed - Win32 Release" "gbak - Win32 Release" "nbackup - Win32 Release" "gpre - Win32 Release" "gsplit - Win32 Release" "gdef - Win32 Release" "gfix - Win32 Release" "gsec - Win32 Release" "gstat - Win32 Release" "instreg - Win32 Release" "instsvc - Win32 Release" "instclient - Win32 Release" "isql - Win32 Release" "qli - Win32 Release" "fbclient - Win32 Release" "fbudf - Win32 Release" "ib_udf - Win32 Release" "ib_util - Win32 Release" "intl - Win32 Release" "fb2control - Win32 Release" %CLEAN% /OUT make_all.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2.sln %CLEAN% release /OUT make_all.log
)
if errorlevel 1 call :ERROR Release build failed
@goto :EOF


::===========
:DEBUG
@echo Building debug
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2.dsw /MAKE "fbserver - Win32 Debug" "fbguard - Win32 Debug" "fb_lock_print - Win32 Debug" "fb_inet_server - Win32 Debug" "fbembed - Win32 Debug" "gbak - Win32 Debug" "nbackup - Win32 Debug" "gpre - Win32 Debug" "gsplit - Win32 Debug" "gdef - Win32 Debug" "gfix - Win32 Debug" "gsec - Win32 Debug" "gstat - Win32 Debug" "instreg - Win32 Debug" "instsvc - Win32 Debug" "instclient - Win32 Release" "isql - Win32 Debug" "qli - Win32 Debug" "fbclient - Win32 Debug" "fbudf - Win32 Debug" "ib_udf - Win32 Debug" "ib_util - Win32 Debug" "intl - Win32 Debug" "fb2control - Win32 Debug" %CLEAN% /OUT make_all.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2.sln %CLEAN% debug /OUT make_all.log
)
if errorlevel 1 call :ERROR Debug build failed
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
::@mkdir %ROOT_PATH%\output\doc
@mkdir %ROOT_PATH%\output\include
@mkdir %ROOT_PATH%\output\lib
@mkdir %ROOT_PATH%\output\system32 > nul

@copy %ROOT_PATH%\extern\icu\bin\icuuc30.dll %ROOT_PATH%\output\bin >nul
@copy %ROOT_PATH%\extern\icu\bin\icudt30.dll %ROOT_PATH%\output\bin >nul
@copy %ROOT_PATH%\extern\icu\bin\icuin30.dll %ROOT_PATH%\output\bin >nul
@copy %ROOT_PATH%\temp\%DBG_DIR%\firebird\bin\* %ROOT_PATH%\output\bin >nul
@copy %ROOT_PATH%\temp\%DBG_DIR%\firebird\intl\* %ROOT_PATH%\output\intl >nul
@copy %ROOT_PATH%\temp\%DBG_DIR%\firebird\udf\* %ROOT_PATH%\output\udf >nul
@copy %ROOT_PATH%\temp\%DBG_DIR%\firebird\system32\* %ROOT_PATH%\output\system32 >nul
@copy %ROOT_PATH%\temp\%DBG_DIR%\fbclient\fbclient.lib %ROOT_PATH%\output\lib\fbclient_ms.lib >nul
:: Firebird.conf, etc
@copy %ROOT_PATH%\gen\firebird.msg %ROOT_PATH%\output > nul
@copy %ROOT_PATH%\builds\install\misc\firebird.conf %ROOT_PATH%\output >nul
@copy %ROOT_PATH%\builds\install\misc\fbintl.conf %ROOT_PATH%\output\intl >nul
:: DATABASES
@copy %ROOT_PATH%\gen\dbs\SECURITY2.FDB %ROOT_PATH%\output\security2.fdb >nul
@copy %ROOT_PATH%\gen\dbs\HELP.fdb %ROOT_PATH%\output\help\help.fdb >nul
::@copy %ROOT_PATH%\gen\firebird.msg %ROOT_PATH%\output\firebird.msg >nul
@copy %ROOT_PATH%\builds\misc\security.gbak %ROOT_PATH%\output\security2.fbk > nul
:: DOCS
::@copy %ROOT_PATH%\ChangeLog %ROOT_PATH%\output\doc\ChangeLog.txt >nul
::@copy %ROOT_PATH%\doc\WhatsNew %ROOT_PATH%\output\doc\WhatsNew.txt >nul

:: HEADERS
:: Don't use this ibase.h unless you have to - we build it better in BuildExecutableInstall.bat
:: This variation doesn't clean up the license templates, and processes the component files in
:: a different order to that used in the production version. However, this version doesn't
:: have a dependancy upon sed while the production one does.
echo #pragma message("Non-production version of ibase.h.") > %ROOT_PATH%\output\include\ibase.tmp
echo #pragma message("Using raw, unprocessed concatenation of header files.") >> %ROOT_PATH%\output\include\ibase.tmp
type %ROOT_PATH%\src\misc\ibase_header.txt >> %ROOT_PATH%\output\include\ibase.tmp
type %ROOT_PATH%\src\include\fb_types.h >> %ROOT_PATH%\output\include\ibase.tmp
type %ROOT_PATH%\src\jrd\dsc_pub.h >> %ROOT_PATH%\output\include\ibase.tmp
type %ROOT_PATH%\src\dsql\sqlda_pub.h >> %ROOT_PATH%\output\include\ibase.tmp
type %ROOT_PATH%\src\jrd\ibase.h >> %ROOT_PATH%\output\include\ibase.tmp
type %ROOT_PATH%\src\jrd\inf_pub.h >> %ROOT_PATH%\output\include\ibase.tmp
type %ROOT_PATH%\src\jrd\blr.h >> %ROOT_PATH%\output\include\ibase.tmp
type %ROOT_PATH%\src\include\gen\iberror.h >> %ROOT_PATH%\output\include\ibase.tmp
sed -f %ROOT_PATH%\src\misc\headers.sed < %ROOT_PATH%\output\include\ibase.tmp > %ROOT_PATH%\output\include\ibase.h
del %ROOT_PATH%\output\include\ibase.tmp > nul

::Copy additional headers
copy %ROOT_PATH%\src\extlib\ib_util.h %ROOT_PATH%\output\include > nul
copy %ROOT_PATH%\src\jrd\perf.h %ROOT_PATH%\output\include >nul
::This is in ibase.h so why make a separate copy?
::copy %ROOT_PATH%\src\jrd\blr.h %ROOT_PATH%\output\include > nul
copy %ROOT_PATH%\src\include\gen\iberror.h %ROOT_PATH%\output\include > nul

:: UDF
copy %ROOT_PATH%\src\extlib\ib_udf.sql %ROOT_PATH%\output\udf > nul
copy %ROOT_PATH%\src\extlib\fbudf\fbudf.sql %ROOT_PATH%\output\udf > nul
:: Examples
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

:ERROR
::====
@echo.
@echo   An error occurred while running make_all.bat -
@echo     %*
@echo.
set ERRLEV=1
cancel_script > nul 2>&1
::End of ERROR
::------------
@goto :EOF




