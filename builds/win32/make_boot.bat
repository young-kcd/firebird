::
:: This bat file don't uses cd, all the paths are full paths.
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
:MAIN
@echo.
@echo Copy autoconfig.h
@del %ROOT_PATH%\src\include\gen\autoconfig.h 2> nul
@copy %ROOT_PATH%\src\include\gen\autoconfig_msvc.h %ROOT_PATH%\src\include\gen\autoconfig.h > nul
@echo Creating directories
:: Don't rmdir gen, it contains the dbs, only the exe files
@del gen\*.exe 2>nul
@rmdir /s /q %ROOT_PATH%\gen\alice 2>nul
@mkdir %ROOT_PATH%\gen\alice 2>nul
@rmdir /s /q %ROOT_PATH%\gen\burp 2>nul
@mkdir %ROOT_PATH%\gen\burp 2>nul
@rmdir /s /q %ROOT_PATH%\gen\dsql 2>nul
@mkdir %ROOT_PATH%\gen\dsql 2>nul
@rmdir /s /q %ROOT_PATH%\gen\dudley 2>nul
@mkdir %ROOT_PATH%\gen\dudley 2>nul
@rmdir /s /q %ROOT_PATH%\gen\gpre 2>nul
@mkdir %ROOT_PATH%\gen\gpre 2>nul
@rmdir /s /q %ROOT_PATH%\gen\isql 2>nul
@mkdir %ROOT_PATH%\gen\isql 2>nul
@rmdir /s /q %ROOT_PATH%\gen\journal 2>nul
@mkdir %ROOT_PATH%\gen\journal 2>nul
@rmdir /s /q %ROOT_PATH%\gen\jrd 2>nul
@mkdir %ROOT_PATH%\gen\jrd 2>nul
@rmdir /s /q %ROOT_PATH%\gen\misc 2>nul
@mkdir %ROOT_PATH%\gen\misc 2>nul
@rmdir /s /q %ROOT_PATH%\gen\msgs 2>nul
@mkdir %ROOT_PATH%\gen\msgs 2>nul
@rmdir /s /q %ROOT_PATH%\gen\qli 2>nul
@mkdir %ROOT_PATH%\gen\qli 2>nul
@rmdir /s /q %ROOT_PATH%\gen\utilities 2>nul
@mkdir %ROOT_PATH%\gen\utilities 2>nul
@mkdir %ROOT_PATH%\gen\utilities\gstat 2>nul
@mkdir %ROOT_PATH%\gen\utilities\gsec 2>nul
@rmdir /s /q %ROOT_PATH%\gen\examples 2>nul
@mkdir %ROOT_PATH%\gen\examples 2>nul

::=======
@call :gpre_boot
if "%ERRLEV%"=="1" goto :END
::=======
@echo Preprocessing the entire source tree...
@call preprocess.bat BOOT
::=======
@call :gpre_static
if "%ERRLEV%"=="1" goto :END
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
@%ROOT_PATH%\gen\build_msg -f %DB_PATH%/gen/firebird.msg -D localhost:%DB_PATH%/gen/dbs/msg.fdb
@%ROOT_PATH%\gen\codes %ROOT_PATH%\src\include\gen %ROOT_PATH%\lang_helpers
::=======
@echo.
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
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project gpre_boot /rebuild release /OUT boot1.log
)
if errorlevel 1 goto :gpre_boot2
@copy %ROOT_PATH%\temp\release\firebird\bin\gpre_boot.exe %ROOT_PATH%\gen\ > nul
goto :EOF

:gpre_boot2
echo.
echo Error building gpre_boot see boot1.log
echo.
set ERRLEV=1
goto :EOF

::===================
:: BUILD gpre_static
:gpre_static
@echo.
@echo Building gpre_static...
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "gpre_static - Win32 Release"  /REBUILD /OUT boot2.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project gpre_static /rebuild release /OUT boot2.log
)
if errorlevel 1 goto :gpre_static2
@copy %ROOT_PATH%\temp\release\firebird\bin\gpre_static.exe   %ROOT_PATH%\gen\ > nul
@goto :EOF
:gpre_static2
echo.
echo Error building gpre_static see boot2.log
echo.
set ERRLEV=1
goto :EOF

::===================
:: BUILD messages
:msgs
@echo.
@echo Building build_msg and codes...
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "build_msg - Win32 Release" /REBUILD /OUT boot3.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project build_msg /rebuild release /OUT boot3.log
)
if errorlevel 1 goto :msgs2
@copy %ROOT_PATH%\temp\release\build_msg\build_msg.exe   %ROOT_PATH%\gen\ > nul
@goto :EOF
:msgs2
echo.
echo Error building messages see boot3.log
echo.
set ERRLEV=1
goto :EOF

::===================
:: BUILD codes
:codes
@echo.
@echo Building build_msg and codes...
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "codes - Win32 Release" /REBUILD /OUT boot4.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project codes  /rebuild release /OUT boot4.log
)
if errorlevel 1 goto :codes2
@copy %ROOT_PATH%\temp\release\codes\codes.exe   %ROOT_PATH%\gen\ > nul
@goto :EOF
:codes2
echo.
echo Error building messages see boot4.log
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
