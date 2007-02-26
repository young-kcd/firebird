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

@call set_build_target.bat %*


::===========
:MAIN
@echo.
@echo Copy autoconfig.h
@del %FB_ROOT_PATH%\src\include\gen\autoconfig.h 2> nul
@copy %FB_ROOT_PATH%\src\include\gen\autoconfig_msvc.h %FB_ROOT_PATH%\src\include\gen\autoconfig.h > nul
@echo Creating directories
@rmdir /s /q %FB_ROOT_PATH%\gen 2>nul
:: Remove previously generated output, and recreate the directory hierarchy. Note the exceptions to the rule!
for %%v in ( alice burp dsql dudley gpre isql journal jrd misc msgs qli examples ) do (
  if NOT "%%v"=="journal" (@mkdir %FB_ROOT_PATH%\gen\%%v )
)

@rmdir /s /q %FB_ROOT_PATH%\gen\utilities 2>nul
@mkdir %FB_ROOT_PATH%\gen\utilities 2>nul
@mkdir %FB_ROOT_PATH%\gen\utilities\gstat 2>nul
@mkdir %FB_ROOT_PATH%\gen\utilities\gsec 2>nul

::=======
call :btyacc
if "%ERRLEV%"=="1" goto :END
@copy %FB_ROOT_PATH%\temp\%FB_OBJ_DIR%\firebird\bin\btyacc.exe %FB_ROOT_PATH%\gen\ > nul

@echo Generating DSQL parser...
@call parse.bat
if "%ERRLEV%"=="1" goto :END

::=======
@echo.
@echo Building BLR Table
@call blrtable.bat

::=======
call :gpre_boot
if "%ERRLEV%"=="1" goto :END
@copy %FB_ROOT_PATH%\temp\%FB_OBJ_DIR%\firebird\bin\gpre_boot.exe %FB_ROOT_PATH%\gen\ > nul

::=======
@echo Preprocessing the source files needed to build gbak_embed, gpre_embed and isql_embed...
@call preprocess.bat BOOT
::=======
call :gbak_embed
if "%ERRLEV%"=="1" goto :END

call :gpre_embed
if "%ERRLEV%"=="1" goto :END

call :isql_embed
if "%ERRLEV%"=="1" goto :END

@copy %FB_ROOT_PATH%\temp\%FB_OBJ_DIR%\firebird\bin\gbak_embed.exe %FB_ROOT_PATH%\gen > nul
@copy %FB_ROOT_PATH%\temp\%FB_OBJ_DIR%\firebird\bin\gpre_embed.exe %FB_ROOT_PATH%\gen > nul
@copy %FB_ROOT_PATH%\temp\%FB_OBJ_DIR%\firebird\bin\isql_embed.exe %FB_ROOT_PATH%\gen > nul
@copy %FB_ROOT_PATH%\temp\%FB_OBJ_DIR%\firebird\bin\fbembed.dll %FB_ROOT_PATH%\gen > nul
@copy %FB_ROOT_PATH%\extern\icu\bin\icuuc30.dll %FB_ROOT_PATH%\gen >nul
@copy %FB_ROOT_PATH%\extern\icu\bin\icudt30.dll %FB_ROOT_PATH%\gen >nul
@copy %FB_ROOT_PATH%\extern\icu\bin\icuin30.dll %FB_ROOT_PATH%\gen >nul

::=======
@call :databases
::=======
@echo Preprocessing the entire source tree...
@call preprocess.bat
::=======
@call :msgs
if "%ERRLEV%"=="1" goto :END
@call :codes
if "%ERRLEV%"=="1" goto :END
::=======
@call create_msgs.bat msg
::=======
@call :NEXT_STEP
@goto :END


::===================
:: BUILD btyacc
:btyacc
@echo.
@echo Building btyacc (%FB_OBJ_DIR%)...
@call compile.bat %FB_ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot btyacc.log btyacc
if errorlevel 1 call :boot2 btyacc
goto :EOF

::===================
:: BUILD gpre_boot
:gpre_boot
@echo.
@echo Building gpre_boot (%FB_OBJ_DIR%)...
@call compile.bat %FB_ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot gpre_boot.log gpre_boot
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
:: BUILD gbak_embed
:gbak_embed
@echo.
@echo Building gbak_embed (%FB_OBJ_DIR%)...
@call compile.bat %FB_ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot gbak_embed.log gbak_embed
if errorlevel 1 call :boot2 gbak_embed
@goto :EOF

::===================
:: BUILD gpre_embed
:gpre_embed
@echo.
@echo Building gpre_embed (%FB_OBJ_DIR%)...
@call compile.bat %FB_ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot gpre_embed.log gpre_embed
if errorlevel 1 call :boot2 gpre_embed
@goto :EOF

::===================
:: BUILD isql_embed
:isql_embed
@echo.
@echo Building isql_embed (%FB_OBJ_DIR%)...
@call compile.bat %FB_ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot isql_embed.log isql_embed
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
@echo Building build_msg (%FB_OBJ_DIR%)...
@call compile.bat %FB_ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot build_msg.log build_msg
if errorlevel 1 goto :msgs2
@copy %FB_ROOT_PATH%\temp\%FB_OBJ_DIR%\build_msg\build_msg.exe   %FB_ROOT_PATH%\gen\ > nul
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
@echo Building codes (%FB_OBJ_DIR%)...
@call compile.bat %FB_ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot codes.log codes
if errorlevel 1 goto :codes2
@copy %FB_ROOT_PATH%\temp\%FB_OBJ_DIR%\codes\codes.exe   %FB_ROOT_PATH%\gen\ > nul
@goto :EOF
:codes2
echo.
echo Error building codes, see codes.log
echo.
set ERRLEV=1
goto :EOF

::==============
:databases
@rmdir /s /q %FB_ROOT_PATH%\gen\dbs 2>nul
@mkdir %FB_ROOT_PATH%\gen\dbs 2>nul

@echo create database '%FB_DB_PATH%\gen\dbs\security2.fdb'; | "%FB_ROOT_PATH%\gen\isql_embed" -q
@"%FB_ROOT_PATH%\gen\isql_embed" -q %FB_DB_PATH%\gen\dbs\security2.fdb -i %FB_ROOT_PATH%\src\dbs\security.sql

@%FB_ROOT_PATH%\gen\gbak_embed -r %FB_ROOT_PATH%\builds\misc\metadata.gbak %FB_DB_PATH%\gen\dbs\metadata.fdb

@call create_msgs.bat db

@%FB_ROOT_PATH%\gen\gbak_embed -r %FB_ROOT_PATH%\builds\misc\help.gbak %FB_DB_PATH%\gen\dbs\help.fdb
@copy %FB_ROOT_PATH%\gen\dbs\metadata.fdb %FB_ROOT_PATH%\gen\dbs\yachts.lnk > nul

@goto :EOF


::==============
:NEXT_STEP
@echo.
@echo    You may now run make_all.bat [DEBUG] [CLEAN]
@echo.
@goto :EOF

:END

