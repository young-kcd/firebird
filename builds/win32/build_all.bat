
@echo off

::===========
:MAIN
@echo.
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
