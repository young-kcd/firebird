:: run prepare.bat first
:: Then run this script.
:: After that run makeX_debug.bat or makeX_release.bat
:: where X is your version of MSVC - 6 or 7.

@echo off

::===========
:MAIN
@echo.
@del ..\..\src\include\gen\autoconfig.h
@copy ..\..\src\include\gen\autoconfig_msvc.h ..\..\src\include\gen\autoconfig.h
@echo Preprocessing files required to do a boot-build...
@call build_boot.bat BOOT
@call preprocess.bat BOOT
@echo.
@echo Making necessary boot-build executables...
@call build_boot.bat
@echo.
@echo Preprocessing the entire source tree...
@call preprocess.bat
@echo Building message file and codes header...
@call build_msg.bat
@goto :END

:END
