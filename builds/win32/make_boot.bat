::
:: This bat file doesn't use cd, all the paths are full paths.
:: with this convention this bat file is position independent
:: and it will be easier to move the place of somefiles.
::

@echo off
set ERRLEV=0

:CHECK_ENV
@call setenvvar.bat
@if errorlevel 1 (goto :END)

:: verify that prepare was run before
@if not exist %ROOT_PATH%\gen\dbs\metadata.fdb (goto :HELP & goto :END)

::===========
:: Read input values
@set DBG=
@set DBG_DIR=release
@set CLEAN=/build
@if "%1"=="DEBUG" ((set DBG=TRUE) && (set DBG_DIR=debug))
@if "%2"=="DEBUG" ((set DBG=TRUE) && (set DBG_DIR=debug))
@if "%1"=="CLEAN" (set CLEAN=/rebuild)
@if "%2"=="CLEAN" (set CLEAN=/rebuild)

::===========
:MAIN
@echo.
@echo Copy autoconfig.h
@del %ROOT_PATH%\src\include\gen\autoconfig.h 2> nul
@copy %ROOT_PATH%\src\include\gen\autoconfig_msvc.h %ROOT_PATH%\src\include\gen\autoconfig.h > nul
@echo Creating directories
:: Don't rmdir gen, it contains the dbs, only the exe files
@del gen\*.exe 2>nul
:: Remove previously generated output, and recreate the directory hierarchy. Note the exceptions to the rule!
for %%v in ( alice burp dsql dudley gpre isql journal jrd misc msgs qli examples ) do (
  @rmdir /s /q %ROOT_PATH%\gen\%%v 2>nul
  if NOT "%%v"=="journal" (@mkdir %ROOT_PATH%\gen\%%v 2>nul)
)

@rmdir /s /q %ROOT_PATH%\gen\utilities 2>nul
@mkdir %ROOT_PATH%\gen\utilities 2>nul
@mkdir %ROOT_PATH%\gen\utilities\gstat 2>nul
@mkdir %ROOT_PATH%\gen\utilities\gsec 2>nul

::=======
@if "%DBG%"=="" (
	call :gpre_boot_release
) else (
	call :gpre_boot_debug
)
if "%ERRLEV%"=="1" goto :END
@copy %ROOT_PATH%\temp\%DBG_DIR%\firebird\bin\gpre_boot.exe %ROOT_PATH%\gen\ > nul
::=======
@echo Preprocessing the source files needed to build gpre_static...
@call preprocess.bat BOOT
::=======
@if "%DBG%"=="" (
	call :gpre_static_release
) else (
	call :gpre_static_debug
)
if "%ERRLEV%"=="1" goto :END
@copy %ROOT_PATH%\temp\%DBG_DIR%\firebird\bin\gpre_static.exe   %ROOT_PATH%\gen\ > nul
::=======
@echo Preprocessing the entire source tree...
@call preprocess.bat
::=======
@call :msgs
if "%ERRLEV%"=="1" goto :END
@call :codes
if "%ERRLEV%"=="1" goto :END
::=======
@echo Building message file and codes header...
@%ROOT_PATH%\gen\build_msg -f %DB_PATH%/gen/firebird.msg -D %SERVER_NAME%:%DB_PATH%/gen/dbs/msg.fdb
@%ROOT_PATH%\gen\codes %ROOT_PATH%\src\include\gen %ROOT_PATH%\lang_helpers
::=======
@echo.
@echo Building BLR Table
@call blrtable.bat
@call :NEXT_STEP
@goto END:


::===================
:: BUILD gpre_boot_release
:gpre_boot_release
@echo.
@echo Building gpre_boot (release)...
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "gpre_boot - Win32 Release"  %CLEAN% /OUT gpre_boot.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project gpre_boot %CLEAN% release /OUT gpre_boot.log
)
if errorlevel 1 goto :gpre_boot2
goto :EOF

::===================
:: BUILD gpre_boot_debug
:gpre_boot_debug
@echo.
@echo Building gpre_boot (debug)...
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "gpre_boot - Win32 Debug"  %CLEAN% /OUT gpre_boot.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project gpre_boot %CLEAN% debug /OUT gpre_boot.log
)
if errorlevel 1 goto :gpre_boot2
goto :EOF

::===================
:: Error gpre_boot
:gpre_boot2
echo.
echo Error building gpre_boot, see gpre_boot.log
echo.
set ERRLEV=1
goto :EOF

::===================
:: BUILD gpre_static_release
:gpre_static_release
@echo.
@echo Building gpre_static (release)...
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "gpre_static - Win32 Release"  %CLEAN% /OUT gpre_static.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project gpre_static %CLEAN% release /OUT gpre_static.log
)
if errorlevel 1 goto :gpre_static2
@goto :EOF

::===================
:: BUILD gpre_static_debug
:gpre_static_debug
@echo.
@echo Building gpre_static (debug)...
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "gpre_static - Win32 Debug"  %CLEAN% /OUT gpre_static.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project gpre_static %CLEAN% debug /OUT gpre_static.log
)
if errorlevel 1 goto :gpre_static2
@goto :EOF

::===================
:: ERROR gpre_static
:gpre_static2
echo.
echo Error building gpre_static, see gpre_static.log
echo.
set ERRLEV=1
goto :EOF


::===================
:: BUILD messages
:msgs
@echo.
@echo Building build_msg...
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "build_msg - Win32 Release" /OUT build_msg.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project build_msg /rebuild release /OUT build_msg.log
)
if errorlevel 1 goto :msgs2
@copy %ROOT_PATH%\temp\release\build_msg\build_msg.exe   %ROOT_PATH%\gen\ > nul
@goto :EOF
:msgs2
echo.
echo Error building build_msg, see build_msg.log
echo.
set ERRLEV=1
goto :EOF

::===================
:: BUILD codes
:codes
@echo.
@echo Building codes...
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "codes - Win32 Release" /OUT codes.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project codes  /rebuild release /OUT codes.log
)
if errorlevel 1 goto :codes2
@copy %ROOT_PATH%\temp\release\codes\codes.exe   %ROOT_PATH%\gen\ > nul
@goto :EOF
:codes2
echo.
echo Error building codes, see codes.log
echo.
set ERRLEV=1
goto :EOF

::==============
:NEXT_STEP
@echo.
@echo    You may now run make_all.bat [DEBUG] [CLEAN]
@echo.
@goto :EOF

::==============
:HELP
@echo.
@echo    You must run prepare.bat before running this script
@echo.
@goto :EOF


:END

