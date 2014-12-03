@echo off
::==============
:: compile.bat solution, output, [projects...]
::   Note: MSVC7/8 don't accept more than one project
::
::   Note2: Our MSVC8 projects create object files in temp/$platform/$config
::     but we call devenv with $config|$platform (note variable in reverse order
::      and odd syntax.) This extended syntax for devenv does not seem to be
::      supported in MSVC7 (despite documentation to the contrary.)

setlocal
set solution=%1
set output=%2
set projects=

@if "%FB_DBG%"=="" (
	if "%VS_VER%"=="msvc6" 	(
		set config=Release
	) else (
		set config=release
	)
) else (
	if "%VS_VER%"=="msvc6" (
		set config=Debug
	) else (
		set config=debug
	)
)
if %MSVC_VERSION% GEQ 8 (
	set config="%config%|%FB_TARGET_PLATFORM%"
)

if "%VS_VER%"=="msvc6" (
	set exec=msdev
) else (
	if "%VS_VER_EXPRESS%"=="1" (
		set exec=vcexpress
	) else (
		set exec=devenv
	)
)

shift
shift

set all_proj=1
:loop_start

if "%1" == "" goto loop_end
set all_proj=0

if "%VS_VER%"=="msvc6" (
	set projects=%projects% /MAKE "%1 - Win32 %config%"
) else (
	set projects=%projects% /project %1
)

shift
goto loop_start

:loop_end

if "%VS_VER%"=="msvc6" (
	if "%all_proj%"=="1" (
		set projects=/MAKE "all - Win32 %config%"
	)
)
if "%VS_VER%"=="msvc6" (
	%exec% %solution%.dsw %projects% %FB_CLEAN% /OUT %output%
) else (
%exec% %solution%.sln %projects% %FB_CLEAN% %config% /OUT %output%
)

endlocal

goto :EOF
