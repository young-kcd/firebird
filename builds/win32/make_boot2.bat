::
:: This bat file doesn't use cd, all the paths are full paths.
:: with this convention this bat file is position independent
:: and it will be easier to move the place of somefiles.
::

@echo off
set ERRLEV=0

:CHECK_ENV
@call setenvvar2.bat
@if errorlevel 1 (goto :END)

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
@rmdir /s /q %ROOT_PATH%\gen 2>nul
:: Remove previously generated output, and recreate the directory hierarchy. Note the exceptions to the rule!
for %%v in ( alice burp dsql dudley gpre isql journal jrd misc msgs qli examples ) do (
  if NOT "%%v"=="journal" (@mkdir %ROOT_PATH%\gen\%%v 2>nul)
)

@rmdir /s /q %ROOT_PATH%\gen\utilities 2>nul
@mkdir %ROOT_PATH%\gen\utilities 2>nul
@mkdir %ROOT_PATH%\gen\utilities\gstat 2>nul
@mkdir %ROOT_PATH%\gen\utilities\gsec 2>nul

::=======
@echo.
@echo Building BLR Table
@call blrtable.bat

::=======
@if "%DBG%"=="" (
	call :gpre_boot_release
) else (
	call :gpre_boot_debug
)
if "%ERRLEV%"=="1" goto :END
@copy %ROOT_PATH%\temp\%DBG_DIR%\firebird\bin\gpre_boot.exe %ROOT_PATH%\gen\ > nul
::=======
@echo Preprocessing the source files needed to build gbak_embed, gpre_embed and isql_embed...
@call preprocess2.bat BOOT
::=======
@if "%DBG%"=="" (
	call :gbak_embed_release
	if "%ERRLEV%"=="1" goto :END
	call :gpre_embed_release
	if "%ERRLEV%"=="1" goto :END
	call :isql_embed_release
	if "%ERRLEV%"=="1" goto :END
) else (
	call :gbak_embed_debug
	if "%ERRLEV%"=="1" goto :END
	call :gpre_embed_debug
	if "%ERRLEV%"=="1" goto :END
	call :isql_embed_debug
	if "%ERRLEV%"=="1" goto :END
)

@copy %ROOT_PATH%\temp\%DBG_DIR%\firebird\bin\gbak_embed.exe %ROOT_PATH%\gen > nul
@copy %ROOT_PATH%\temp\%DBG_DIR%\firebird\bin\gpre_embed.exe %ROOT_PATH%\gen > nul
@copy %ROOT_PATH%\temp\%DBG_DIR%\firebird\bin\isql_embed.exe %ROOT_PATH%\gen > nul
@copy %ROOT_PATH%\temp\%DBG_DIR%\firebird\bin\fbembed.dll %ROOT_PATH%\gen > nul
@copy %ROOT_PATH%\extern\icu\bin\icuuc30.dll %ROOT_PATH%\gen >nul
@copy %ROOT_PATH%\extern\icu\bin\icudt30.dll %ROOT_PATH%\gen >nul
@copy %ROOT_PATH%\extern\icu\bin\icuin30.dll %ROOT_PATH%\gen >nul

::=======
@call :databases
::=======
@echo Preprocessing the entire source tree...
@call preprocess2.bat
::=======
@call :msgs
if "%ERRLEV%"=="1" goto :END
@call :codes
if "%ERRLEV%"=="1" goto :END
::=======
@echo Building message file and codes header...
@%ROOT_PATH%\gen\build_msg -f %DB_PATH%/gen/firebird.msg -D %DB_PATH%/gen/dbs/msg.fdb
@%ROOT_PATH%\gen\codes %ROOT_PATH%\src\include\gen %ROOT_PATH%\lang_helpers
::=======
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
:: BUILD gbak_embed_release
:gbak_embed_release
@echo.
@echo Building gbak_embed (release)...
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "gbak_embed - Win32 Release"  %CLEAN% /OUT gbak_embed.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project gbak_embed %CLEAN% release /OUT gbak_embed.log
)
if errorlevel 1 call :boot2 gbak_embed
@goto :EOF

::===================
:: BUILD gbak_embed_debug
:gbak_embed_debug
@echo.
@echo Building gbak_embed (debug)...
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "gbak_embed - Win32 Debug"  %CLEAN% /OUT gbak_embed.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project gbak_embed %CLEAN% debug /OUT gbak_embed.log
)
if errorlevel 1 call :boot2 gbak_embed
@goto :EOF


::===================
:: BUILD gpre_embed_release
:gpre_embed_release
@echo.
@echo Building gpre_embed (release)...
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "gpre_embed - Win32 Release"  %CLEAN% /OUT gpre_embed.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project gpre_embed %CLEAN% release /OUT gpre_embed.log
)
if errorlevel 1 call :boot2 gpre_embed
@goto :EOF

::===================
:: BUILD gpre_embed_debug
:gpre_embed_debug
@echo.
@echo Building gpre_embed (debug)...
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "gpre_embed - Win32 Debug"  %CLEAN% /OUT gpre_embed.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project gpre_embed %CLEAN% debug /OUT gpre_embed.log
)
if errorlevel 1 call :boot2 gpre_embed
@goto :EOF


::===================
:: BUILD isql_embed_release
:isql_embed_release
@echo.
@echo Building isql_embed (release)...
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "isql_embed - Win32 Release"  %CLEAN% /OUT isql_embed.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project isql_embed %CLEAN% release /OUT isql_embed.log
)
if errorlevel 1 call :boot2 isql_embed
@goto :EOF

::===================
:: BUILD isql_embed_debug
:isql_embed_debug
@echo.
@echo Building isql_embed (debug)...
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.dsw /MAKE "isql_embed - Win32 Debug"  %CLEAN% /OUT isql_embed.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project isql_embed %CLEAN% debug /OUT isql_embed.log
)
if errorlevel 1 call :boot2 isql_embed
@goto :EOF


::===================
:: ERROR boot
:boot2
echo.
echo Error building %1, see %1.log
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
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project build_msg %CLEAN% release /OUT build_msg.log
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
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot.sln /project codes %CLEAN% release /OUT codes.log
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
:databases
@rmdir /s /q %ROOT_PATH%\gen\dbs 2>nul
@mkdir %ROOT_PATH%\gen\dbs 2>nul

@echo create database '%DB_PATH%\gen\dbs\security2.fdb'; | "%ROOT_PATH%\gen\isql_embed" -q
@"%ROOT_PATH%\gen\isql_embed" -q %DB_PATH%\gen\dbs\security2.fdb -i %ROOT_PATH%\src\dbs\security.sql

@%ROOT_PATH%\gen\gbak_embed -r %ROOT_PATH%\builds\misc\metadata.gbak %DB_PATH%\gen\dbs\metadata.fdb

@echo create database '%DB_PATH%\gen\dbs\msg.fdb'; | "%ROOT_PATH%\gen\isql_embed" -q
@set MSG_ISQL=@"%ROOT_PATH%\gen\isql_embed" -q %DB_PATH%\gen\dbs\msg.fdb -i %ROOT_PATH%\src\msgs\
@%MSG_ISQL%msg.sql
@%MSG_ISQL%facilities.sql
@echo.
@echo loading locales
@%MSG_ISQL%locales.sql
@echo loading history
@%MSG_ISQL%history.sql
@echo loading messages
@%MSG_ISQL%messages.sql
@echo loading symbols
@%MSG_ISQL%symbols.sql
@echo loading system errors
@%MSG_ISQL%system_errors.sql
@echo loading French translation
@%MSG_ISQL%transmsgs.fr_FR.sql
@echo loading German translation
@%MSG_ISQL%transmsgs.de_DE.sql

@%ROOT_PATH%\gen\gbak_embed -r %ROOT_PATH%\builds\misc\help.gbak %DB_PATH%\gen\dbs\help.fdb
@copy %ROOT_PATH%\gen\dbs\metadata.fdb %ROOT_PATH%\gen\dbs\yachts.lnk > nul

@goto :EOF


::==============
:NEXT_STEP
@echo.
@echo    You may now run make_all.bat [DEBUG] [CLEAN]
@echo.
@goto :EOF

:END

