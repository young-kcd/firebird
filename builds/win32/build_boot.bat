
@echo off

::===========
:MAIN
@call :NMAKE common_boot.mak
@call :NMAKE gpre_boot.mak
@call :NMAKE fbclient_boot.mak
@call :NMAKE gpre.mak
@goto :EOF

::===========
:NMAKE
@nmake /f %1
@goto :EOF

:END
