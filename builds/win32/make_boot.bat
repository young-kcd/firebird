::
:: This bat file don't uses cd, all the paths are full paths.
:: with this convention this bat file is position independent 
:: and it will be easier to move the place of somefiles.
::

@echo off

:CHECK_ENV
@call setenvvar.bat
@if errorlevel 1 (goto :END)

:: verify that prepare was run before
@if not exist %ROOT_PATH%\builds\win32\dbs\metadata.fdb (goto :HELP & goto :END)

::===========
:MAIN
@echo.
@echo Copy autoconfig.h
@del %ROOT_PATH%\src\include\gen\autoconfig.h 2> nul
@copy %ROOT_PATH%\src\include\gen\autoconfig_msvc.h %ROOT_PATH%\src\include\gen\autoconfig.h > nul
@echo Creating directories
@rmdir /s /q %ROOT_PATH%\gen 2>nul
@mkdir %ROOT_PATH%\gen\alice 2>nul
@mkdir %ROOT_PATH%\gen\burp 2>nul
@mkdir %ROOT_PATH%\gen\dsql 2>nul
@mkdir %ROOT_PATH%\gen\dudley 2>nul
@mkdir %ROOT_PATH%\gen\gpre 2>nul
@mkdir %ROOT_PATH%\gen\isql 2>nul
@mkdir %ROOT_PATH%\gen\jrd 2>nul
@mkdir %ROOT_PATH%\gen\msgs 2>nul
@mkdir %ROOT_PATH%\gen\qli 2>nul
@mkdir %ROOT_PATH%\gen\utilities 2>nul
@mkdir %ROOT_PATH%\gen\v5_examples 2>nul

::provisional, copy all fdbs to the same directory to be used from preprocess
copy %ROOT_PATH%\builds\win32\dbs\metadata.fdb   %ROOT_PATH%\gen\yachts.lnk > nul
copy %ROOT_PATH%\builds\win32\dbs\jrd\security.fdb   %ROOT_PATH%\gen\security.fdb > nul
copy %ROOT_PATH%\builds\win32\dbs\msgs\msg.fdb   %ROOT_PATH%\gen\msg.fdb > nul
copy %ROOT_PATH%\builds\win32\dbs\qli\help.fdb   %ROOT_PATH%\gen\help.fdb > nul
::=======
@call :gpre_boot
::=======
@echo Preprocessing the entire source tree...
@call preprocess.bat BOOT
::=======
@call :gpre_static
::=======
@echo Preprocessing the entire source tree...
@call preprocess.bat
::=======
@call :msgs
::=======
@echo Building message file and codes header...
@%ROOT_PATH%\gen\build_msg -D %DB_PATH%/gen/msg.fdb
@%ROOT_PATH%\gen\codes %ROOT_PATH%\src\include\gen
::=======
@echo Building BLR Table
@call blrtable.bat
@call :NEXT_STEP
@goto END:


::===================
:: BUILD gpre_boot
:gpre_boot
@echo.
@echo Building gpre_boot...
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "gpre_boot - Win32 Release"  /REBUILD /OUT boot1.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project gpre_boot /rebuild /OUT boot1.log
)
@copy %ROOT_PATH%\builds\win32\%VS_VER%\release\firebird\bin\gpre_boot.exe %ROOT_PATH%\gen\ > nul
goto :EOF

::===================
:: BUILD gpre_static
:gpre_static
@echo.
@echo Building gpre_static...
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "gpre_static - Win32 Release"  /REBUILD /OUT boot2.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project gpre_static /REBUILD /OUT boot2.log
)
@copy %ROOT_PATH%\builds\win32\%VS_VER%\release\firebird\bin\gpre_static.exe   %ROOT_PATH%\gen\ > nul
@goto :EOF


::===================
:: BUILD messages and codes
:msgs
@echo.
@echo Building build_msg and codes...
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "build_msg - Win32 Release" "codes - Win32 Release"  /REBUILD /OUT boot3.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project build_msg /REBUILD /OUT boot3.log
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project codes  /REBUILD /OUT boot4.log
)
@copy %ROOT_PATH%\builds\win32\%VS_VER%\release\build_msg\build_msg.exe   %ROOT_PATH%\gen\ > nul
@copy %ROOT_PATH%\builds\win32\%VS_VER%\release\codes\codes.exe   %ROOT_PATH%\gen\ > nul
@goto :EOF

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
