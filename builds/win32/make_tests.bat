@echo off
set ERRLEV=0

:: Set env vars
@call setenvvar.bat

@if errorlevel 1 (call :ERROR Executing setenvvar.bat failed & goto :EOF)

:: verify that boot was run before

@if not exist %FB_GEN_DIR%\firebird.msg (goto :HELP_BOOT & goto :EOF)


@call set_build_target.bat %*

::==========
:: MAIN

@echo Building %FB_OBJ_DIR%

call compile.bat builds\win32\%VS_VER%\FirebirdTests make_tests_%FB_TARGET_PLATFORM%.log
if errorlevel 1 call :ERROR build failed - see make_tests_%FB_TARGET_PLATFORM%.log for details

@goto :EOF
