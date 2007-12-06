::
:: This bat file doesn't use current working directory.  
:: All the paths are full paths. With this convention 
:: this bat file is position independent and it will be 
:: easier to move the place of some files.
::

@echo off

:CHECK_ENV
@call setenvvar.bat
@if errorlevel 1 (goto :END)

:: verify that prepare was run before
@if not exist %ROOT_PATH%\gen\dbs\metadata.fdb ((goto :HELP ) & ( goto :END))

::===========
:MAIN
@echo.
@echo Copy autoconfig.h
@del %ROOT_PATH%\src\include\gen\autoconfig.h 2> nul
@copy %ROOT_PATH%\src\include\gen\autoconfig_msvc.h %ROOT_PATH%\src\include\gen\autoconfig.h > nul
@echo Creating directories under %ROOT_PATH%\gen
for %%a in ( alice burp dsql dudley gpre isql jrd msgs qli utilities v5_examples) do (
  rmdir /s /q %ROOT_PATH%\gen\%%a 2>nul
  mkdir %ROOT_PATH%\gen\%%a 2>nul
)

::=======
@call :gpre_boot || ((call :ERROR calling gpre_boot subroutine) & (goto :EOF))
::=======
@echo Preprocessing the entire source tree...
@call preprocess.bat BOOT || ((call :ERROR calling preprocess.bat with BOOT param ) & ( goto :EOF))
::=======
@call :gpre_static || ((call :ERROR calling gpre_static subroutine ) & ( goto :EOF))
::=======
@echo Preprocessing the entire source tree...
@call preprocess.bat || ((call :ERROR calling preprocess.bat ) & ( goto :EOF))
::=======
@call :msgs
::=======
@echo Building message file and codes header...
@%ROOT_PATH%\gen\build_msg -f %DB_PATH%/gen/firebird.msg -D localhost:%DB_PATH%/gen/dbs/msg.fdb
@%ROOT_PATH%\gen\codes %ROOT_PATH%\src\include\gen
::=======
@echo Building BLR Table
@call blrtable.bat || ((call :ERROR calling blrtable.bat with BOOT param ) & ( goto :EOF))
@call :NEXT_STEP
@goto END:


::===================
:: BUILD gpre_boot
:gpre_boot
@echo.
@echo Building gpre_boot...
if "%VS_VER%"=="msvc6" (
echo Root_path is %ROOT_PATH%
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "gpre_boot - Win32 Release"  /REBUILD /OUT make_boot1_gpre_boot.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project gpre_boot /rebuild release /OUT make_boot1_gpre_boot.log
)
@copy %ROOT_PATH%\temp\release\firebird\bin\gpre_boot.exe %ROOT_PATH%\gen\ > nul
goto :EOF

::===================
:: BUILD gpre_static
:gpre_static
@echo.
@echo Building gpre_static...
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "gpre_static - Win32 Release"  /REBUILD /OUT make_boot2_gpre_static.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project gpre_static /rebuild release /OUT make_boot2_gpre_static.log
)
@copy %ROOT_PATH%\temp\release\firebird\bin\gpre_static.exe   %ROOT_PATH%\gen\ > nul
@goto :EOF


::===================
:: BUILD messages and codes
:msgs
@echo.
@echo Building build_msg and codes...
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "build_msg - Win32 Release" "codes - Win32 Release"  /REBUILD /OUT make_boot3_msg_codes.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project build_msg /rebuild release /OUT make_boot3_build_msg.log
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project codes  /rebuild release /OUT make_boot4_codes.log
)
@copy %ROOT_PATH%\temp\release\build_msg\build_msg.exe   %ROOT_PATH%\gen\ > nul
@copy %ROOT_PATH%\temp\release\codes\codes.exe   %ROOT_PATH%\gen\ > nul
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


:ERROR
@echo.
@echo Oops - something went wrong.
@echo There was an error %*
@echo.
goto :EOF

:END
