
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
@goto :END

:END
