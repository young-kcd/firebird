
@echo off

::===========
:MAIN
@echo.
@echo Preprocessing files required to do a boot-build...
@preprocess.bat BOOT
@echo.
@echo Making necessary boot-build executables...
@build_boot.bat
@echo.
@echo Preprocessing the entire source tree...
@preprocess.bat
@goto :EOF

:END
