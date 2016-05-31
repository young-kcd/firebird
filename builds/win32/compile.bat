@echo off
::==============
:: compile.bat solution, output, [projects...]
::
::   Note: Our projects create object files in temp/$platform/$config
::     but we call devenv with $config|$platform (note variable in reverse order
::     and odd syntax).

setlocal
set solution=%1
set output=%2
set projects=

@if "%FB_DBG%"=="" (
	set config=release
) else (
	set config=debug
)

set config="%config%|%FB_TARGET_PLATFORM%"

if "%VS_VER_EXPRESS%"=="1" (
	set exec=vcexpress
) else (
	set exec=devenv
)

shift
shift

:loop_start

if "%1" == "" goto loop_end

set projects=%projects% /project %1

shift
goto loop_start

:loop_end

%exec% %solution%.sln %projects% %FB_CLEAN% %config% /OUT %output%

endlocal

goto :EOF
