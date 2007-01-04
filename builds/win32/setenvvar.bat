:: This bat sets the environment values
:: ROOT_PATH dos format path of the main directory
:: DB_PATH unix format path of the main directory
:: VS_VER VisualStudio version (msvc6|msvc7|msvc8)
:: SERVER_NAME server needed to connect to firebird (could include port)
::   Example : localhost/3051
:: (Note - SERVER_NAME is almost deprecated - it is only used by
::   make_examples.bat


@echo off
::=================
:SET_DB_DIR

@SET SERVER_NAME=localhost

@set PLATFORM=win32
@if %PROCESSOR_ARCHITECTURE%==x86 (set PLATFORM=win32)
@if %PROCESSOR_ARCHITECTURE%==AMD64 (set PLATFORM=x64)

@cd ..\..
@for /f "delims=" %%a in ('@cd') do (set ROOT_PATH=%%a)
@cd %~dp0
@for /f "tokens=*" %%a in ('@echo %ROOT_PATH:\=/%') do (set DB_PATH=%%a)

@vcexpress /? >nul 2>nul
@if not errorlevel 9009 ((set MSVC_VERSION=8) & (set VS_VER=msvc8) & (set VS_VER_EXPRESS=1) & (goto :END))

@set VS_VER_EXPRESS=

@for /f "delims=." %%a in ('@devenv /?') do (
  @for /f "tokens=6" %%b in ("%%a") do ((set MSVC_VERSION=%%b) & (set VS_VER=msvc%%b) & (goto :END))
)

@msdev /? >nul 2>nul
@if not errorlevel 9009 ((set MSVC_VERSION=6) & (set VS_VER=msvc6) & (goto :END))


::===========
:HELP
@echo.
@echo    ERROR:
@echo    A working version of Visual Studio cannot be found
@echo    on your current path.
@echo.
@echo    You need MS Visual Studio 6, 7 or 8 to build Firebird
@echo    from these batch files.
@echo.
@echo    Perhaps you need to run something like:
@echo.
@echo      %%VS71COMNTOOLS%%\vsvars32.bat
@echo.
@echo    or
@echo      %%VS80COMNTOOLS%%\vsvars32.bat
@echo.
@echo    depending on the version of Visual Studio.
@echo.
:: set errorlevel
@exit /B 1

:END
@echo    Setting Environment Variables thus...
@echo.
@echo    vs_ver=%VS_VER%
@echo    vs_ver_express=%VS_VER_EXPRESS%
@echo    platform=%PLATFORM%
@echo    msvc_version=%MSVC_VERSION%
@echo    db_path=%DB_PATH%
@echo    root_path=%ROOT_PATH%
@echo    server_name=%SERVER_NAME%
@echo.
@echo    (End of %0)

@exit /B 0
