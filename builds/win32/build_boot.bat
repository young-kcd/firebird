:: this is called automatically by build_all.bat
::
@echo off

::===========
:MAIN
@if "%1"=="BOOT" (@call :MAKE_BOOT) else (@call :MAKE)
@goto :END

::===========
:MAKE_BOOT
@echo Calling common_boot.mak
@call :NMAKE common_boot.mak
@echo Calling gpre_boot.mak
@call :NMAKE gpre_boot.mak
@goto :EOF

::===========
:MAKE
@call :NMAKE fbclient_boot.mak
@call :NMAKE gpre.mak
@goto :EOF

::===========
:NMAKE
@nmake /f %1
@goto :EOF

:END
