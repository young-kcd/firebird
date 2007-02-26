@echo off
set FB_DBG=
set FB_OBJ_DIR=release
set FB_CLEAN=/build
set FB_ICU=

for %%v in ( %* )  do (
  ( if /I "%%v"=="DEBUG" ( (set FB_DBG=TRUE) && (set FB_OBJ_DIR=debug) ) )
  ( if /I "%%v"=="CLEAN" (set FB_CLEAN=/rebuild) )
  ( if /I "%%v"=="ICU" (set FB_ICU=1) )
)

::If we are build icu then we want to test for a platform switch.
::If found we must rebuild.
if defined FB_ICU (
  if exist make_icu_win32.log (

	if "%PROCESSOR_ARCHITECTURE%"=="AMD64" ((set FB_CLEAN=/rebuild) & (@echo Rebuilding icu))
  )
  if exist make_icu_x64.log (
	if "%PROCESSOR_ARCHITECTURE%"=="x86" ((set FB_CLEAN=/rebuild) & (@echo Rebuilding icu))
  )
)

::For now, icu continues to use the old object directory hierarchy
if not defined FB_ICU (
if %MSVC_VERSION% GEQ 8 (set FB_OBJ_DIR=%FB_TARGET_PLATFORM%\%FB_OBJ_DIR%)
)

@echo    Executed %0
@echo.

