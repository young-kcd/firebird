@echo off

::==============
:: compile.bat solution, output, [projects...]
::   Note: MSVC7/8 don't accept more than one project

setlocal

set solution=%1
set output=%2
set projects=""

@if "%DBG%"=="" (
	if "%VS_VER%"=="msvc6" (
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

if "%VS_VER%"=="msvc6" (
	set exec=msdev
) else (
	if "%VS_VER_EXPRESS%"=="1" (
		set exec=vcexpress
	) else (
		set exec=devenv
	)

	if "%VS_VER%"=="msvc8" (
		set config="%config%|%PLATFORM%"
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
	%exec% %solution%.dsw %projects% %CLEAN% /OUT %output%
) else (
	%exec% %solution%.sln %projects% %CLEAN% %config% /OUT %output%
)

endlocal

goto :EOF
