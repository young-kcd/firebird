:: This bat sets the environment values
:: ROOT_PATH dos format path of the main directory
:: DB_PATH unix format path of the main directory
:: VS_VER VisualStudio version (msvc6|msvc7)
:: SERVER_NAME server needed to connect to firebird (could include port)
::   Example : localhost/3051

@echo off
::=================
:SET_DB_DIR

@SET SERVER_NAME=localhost

@cd ..\..
@for /f "delims=" %%a in ('@cd') do (set ROOT_PATH=%%a)
@cd %~dp0
for /f "tokens=*" %%a in ('@echo %ROOT_PATH:\=/%') do (set DB_PATH=%%a)

@msdev /? >nul 2>nul
@if not errorlevel 9009 ((set MSVC_VERSION=6) & (set VS_VER=msvc6) & (goto :END))

@devenv /? >nul 2>nul
@if not errorlevel 9009 ((set MSVC_VERSION=7) & (set VS_VER=msvc7) & (goto :END))

::===========
:HELP
@echo.
@echo    ERROR: A working version of visual studio cannot be found on your current path.
@echo    You need MS Visual Studio 6 or 7 to build Firebird from these batch files.
@echo.
:: set errorlevel
@exit /B 1

:END
@echo.
@echo    vs_ver=%VS_VER%
@echo    msvc_version=%MSVC_VERSION%
@echo    db_path=%DB_PATH%
@echo    root_path=%ROOT_PATH%
@echo    firebird=%FIREBIRD%
@echo    server_name=%SERVER_NAME%
@echo.
