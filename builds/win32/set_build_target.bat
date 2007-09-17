@echo off
set FB_DBG=
set FB_OBJ_DIR=release
set FB_CLEAN=/build
set FB_ICU=

for %%v in ( %* )  do (
  ( if /I "%%v"=="DEBUG" ( (set FB_DBG=TRUE) && (set FB_OBJ_DIR=debug) ) )
  ( if /I "%%v"=="CLEAN" (set FB_CLEAN=/rebuild) )
  ( if /I "%%v"=="ICU" ( (set FB_ICU=1) && (set FB_DBG=) ) )
)

if %MSVC_VERSION% GEQ 8 (set FB_OBJ_DIR=%FB_TARGET_PLATFORM%\%FB_OBJ_DIR%)

@echo    Executed %0
@echo.

