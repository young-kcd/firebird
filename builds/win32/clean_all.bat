@echo off

@call setenvvar.bat
@if errorlevel 1 (goto :END)

@echo Cleaning temporary directories
@rmdir /S /Q %ROOT_PATH%\output 2>nul
@rmdir /S /Q %ROOT_PATH%\gen 2>nul
@rmdir /S /Q %ROOT_PATH%\temp 2>nul

@echo Cleaning INTL
@rmdir /S /Q %ROOT_PATH%\extern\icu\bin
@rmdir /S /Q %ROOT_PATH%\extern\icu\include
@rmdir /S /Q %ROOT_PATH%\extern\icu\lib
@rmdir /S /Q %ROOT_PATH%\extern\icu\source\data\out
@del /Q %ROOT_PATH%\extern\icu\source\extra\uconv\resources\*.res
@del %ROOT_PATH%\extern\icu\source\extra\uconv\pkgdatain.txt
@del %ROOT_PATH%\extern\icu\source\stubdata\stubdatabuilt.txt
@rmdir /S /Q %ROOT_PATH%\extern\icu\source\test\testdata\out
@rmdir /S /Q %ROOT_PATH%\extern\icu\source\tools\gencmn\decmn_Win32_Release
@rmdir /S /Q %ROOT_PATH%\extern\icu\source\tools\genrb\derb_Win32_Release

@echo Cleaning and install build files
@del *.log 2>nul
@del %ROOT_PATH%\src\include\gen\autoconfig.h 2>nul
@del %ROOT_PATH%\src\include\gen\blrtable.h 2>nul
@del %ROOT_PATH%\builds\win32\msvc6\*.plg 2>nul
@del %ROOT_PATH%\builds\win32\msvc7\*.plg 2>nul
@del %ROOT_PATH%\builds\install\arch-specific\win32\*.dll 2>nul
@rmdir /s /q %ROOT_PATH%\builds\win32\install_image 2>nul

